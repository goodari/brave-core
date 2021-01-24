/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_LEDGER_BAT_LEDGER_CONTEXT_H_
#define BAT_LEDGER_BAT_LEDGER_CONTEXT_H_

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "base/location.h"
#include "bat/ledger/ledger.h"
#include "bat/ledger/public/interfaces/ledger.mojom.h"

namespace ledger {

class LedgerImpl;
class LedgerClient;

class BATLedgerContext {
 public:
  using ComponentKey = unsigned;

  class Component {
   public:
    virtual ~Component();

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

   protected:
    explicit Component(BATLedgerContext* context);

    BATLedgerContext* context() { return context_; }

   private:
    // A pointer to the BATLedgerContext instance that owns this object
    BATLedgerContext* context_;
  };

  // TODO(zenparsing): Some of the "levels" don't really make a whole lot
  // of sense. We should review. Can we just have Error, Info, Verbose, and
  // Full? We should remove the other log levels and reset these number.
  enum class LogLevel { kError = 0, kInfo = 1, kVerbose = 6, kFull = 9 };

  class LogStream {
   public:
    LogStream(BATLedgerContext* context,
              base::Location location,
              LogLevel log_level);

    LogStream(const LogStream&) = delete;
    LogStream& operator=(const LogStream&) = delete;

    LogStream(LogStream&& other);
    LogStream& operator=(LogStream&& other);

    ~LogStream();

    template <typename T>
    LogStream& operator<<(const T& value) {
      stream_ << value;
      return *this;
    }

   private:
    BATLedgerContext* context_;
    base::Location location_;
    LogLevel log_level_;
    std::ostringstream stream_;
    bool moved_ = false;
  };

  explicit BATLedgerContext(LedgerImpl* ledger_impl);
  ~BATLedgerContext();

  BATLedgerContext(const BATLedgerContext&) = delete;
  BATLedgerContext& operator=(const BATLedgerContext&) = delete;

  LedgerImpl* GetLedgerImpl() const { return ledger_impl_; }
  LedgerClient* GetLedgerClient() const { return ledger_client_; }

  template <typename T>
  T* Get() {
    static_assert(std::is_base_of<Component, T>::value,
                  "Get requires a subclass of Component");
    auto iter = components_.find(T::kComponentKey);
    if (iter != components_.end())
      return reinterpret_cast<T*>(iter->second.get());

    T* instance = new T(this);
    components_[T::kComponentKey] = std::unique_ptr<T>(instance);
    return instance;
  }

  template <typename T>
  void SetComponent(std::unique_ptr<T> component) {
    static_assert(std::is_base_of<Component, T>::value,
                  "SetComponent requires a subclass of Component");
    components_[T::kComponentKey] = std::move(component);
  }

  static ComponentKey ReserveComponentKey();

  LogStream Log(base::Location location, LogLevel log_level);
  LogStream LogError(base::Location location);
  LogStream LogInfo(base::Location location);
  LogStream LogVerbose(base::Location location);
  LogStream LogFull(base::Location location);

  template <typename T>
  const T& GetSettings() {
    switch (ledger::_environment) {
      case mojom::Environment::DEVELOPMENT: return T::kDevelopment;
      case mojom::Environment::STAGING: return T::kStaging;
      case mojom::Environment::PRODUCTION: return T::kProduction;
    }
  }

 private:
  LedgerImpl* ledger_impl_;
  LedgerClient* ledger_client_;
  std::map<ComponentKey, std::unique_ptr<Component>> components_;
};

}  // namespace ledger

#endif  // BAT_LEDGER_BAT_LEDGER_CONTEXT_H_
