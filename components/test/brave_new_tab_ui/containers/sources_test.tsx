import * as React from 'react';
import { shallow } from 'enzyme'

import { Category } from '../../../brave_new_tab_ui/containers/newTab/settings/braveToday/sources';
import { Toggle } from '../../../brave_new_tab_ui/components/toggle';
import { SettingsSectionTitle } from "../../../brave_new_tab_ui/components/default";

describe('sources.tsx', () => {
  it("settings section renders", () => {
    const props = {
      category: "",
      publishers: [],
      onBack: () => null,
      setPublisherPref: (publisherId: string, enabled: boolean) => null,
    };
    const wrapper = shallow(<Category
      category={props.category}
      publishers={props.publishers}
      onBack={props.onBack}
      setPublisherPref={props.setPublisherPref}
    />)
    expect(wrapper.find(SettingsSectionTitle)).toHaveLength(1);
  });
  it("toggle button renders", () => {
    const props = {
      category: "",
      publishers: [],
      onBack: () => null,
      setPublisherPref: (publisherId: string, enabled: boolean) => null,
    };
    const wrapper = shallow(<Category
      category={props.category}
      publishers={props.publishers}
      onBack={props.onBack}
      setPublisherPref={props.setPublisherPref}
    />)
    expect(wrapper.find(Toggle)).toHaveLength(1);
  });
});