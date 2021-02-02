/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { LocaleContext } from '../localeContext'
import { DialogTitle } from '../dialogTitle'
import { OrderSummary } from '../orderSummary'
import { UseWalletPanel } from '../useWalletPanel'

interface PaymentMethodPanelProps {
  rewardsEnabled: boolean
  orderDescription: string
  orderTotal: string
  orderTotalConverted: string
  hasSufficientFunds: boolean
  walletBalance: string
  walletBalanceConverted: string
  walletVerified: boolean
  walletLastUpdated: string
  onPayWithWallet: () => void
}

export function PaymentMethodPanel (props: PaymentMethodPanelProps) {
  const locale = React.useContext(LocaleContext)

  return (
    <>
      <DialogTitle>{locale.get('paymentMethodTitle')}</DialogTitle>
      <OrderSummary
        description={props.orderDescription}
        orderTotal={props.orderTotal}
        orderTotalConverted={props.orderTotalConverted}
      />
      {
          <UseWalletPanel
            balance={props.walletBalance}
            balanceConverted={props.walletBalanceConverted}
            lastUpdated={props.walletLastUpdated}
            hasSufficientFunds={props.hasSufficientFunds}
            rewardsEnabled={props.rewardsEnabled}
            walletVerified={props.walletVerified}
            onPayWithWallet={props.onPayWithWallet}
          />
      }
    </>
  )
}
