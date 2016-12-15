calibration_parameters {
  uint8_t ampratio3;
  uint8_t ampratio2;
  uint8_t ampratio1;
  uint16_t dark_current;
  uint16_t ampoffset3;
  uint16_t ampoffset2;
  uint16_t ampoffset1;
  uint16_t switch_threshold;
  uint16_t calib_ratio;
}

user_settings {
  uint8_t averaging_time;
  uint8_t zero_measurment_time;
  uint8_t collection_time;
  uint8_t integration_time;
  uint16_t lower_threshold;
  uint16_t upper_threshold;
}

device_number {
  uint32_t device_number;
}

calibration_date {
  char string[25];
}

operating_time {
  uint16_t lamp_hours_1;
  uint16_t lamp_hours_2;
}

linearization {
  uint16_t linearization;
}

device_configuration {
  uint8_t language;
  uint8_t device_configuration;
}

device_type {
  uint8_t device_type;
}

valve_settings {
  uint8_t valve_off;
  uint8_t valve_on;
}

temp_ratio_test_cell {
  uint16_t fractional;
  uint16_t main;
}
