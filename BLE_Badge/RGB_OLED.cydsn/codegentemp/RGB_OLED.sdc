# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\ROUSSMA\Documents\PSoC Creator\RGB_OLED\RGB_OLED.cydsn\RGB_OLED.cyprj
# Date: Thu, 28 Dec 2017 15:28:35 GMT
#set_units -time ns
create_clock -name {Capsense_SampleClk(FFB)} -period 21250 -waveform {0 10625} [list [get_pins {ClockBlock/ff_div_5}]]
create_clock -name {Capsense_SenseClk(FFB)} -period 21250 -waveform {0 10625} [list [get_pins {ClockBlock/ff_div_4}]]
create_clock -name {SPI_Master_SCBCLK(FFB)} -period 83.333333333333329 -waveform {0 41.6666666666667} [list [get_pins {ClockBlock/ff_div_1}]]
create_clock -name {Clock_2(FFB)} -period 125000 -waveform {0 62500} [list [get_pins {ClockBlock/ff_div_7}]]
create_clock -name {SCB_SCBCLK(FFB)} -period 83.333333333333329 -waveform {0 41.6666666666667} [list [get_pins {ClockBlock/ff_div_2}]]
create_clock -name {Clock_1(FFB)} -period 83.333333333333329 -waveform {0 41.6666666666667} [list [get_pins {ClockBlock/ff_div_10}]]
create_clock -name {CyRouted1} -period 83.333333333333329 -waveform {0 41.6666666666667} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyWCO} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/wco}]]
create_clock -name {CyLFCLK} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyECO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/eco}]]
create_clock -name {CyIMO} -period 83.333333333333329 -waveform {0 41.6666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFCLK} -period 83.333333333333329 -waveform {0 41.6666666666667} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySYSCLK} -period 83.333333333333329 -waveform {0 41.6666666666667} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {Capsense_SampleClk} -source [get_pins {ClockBlock/hfclk}] -edges {1 255 511} [list]
create_generated_clock -name {Capsense_SenseClk} -source [get_pins {ClockBlock/hfclk}] -edges {1 255 511} [list]
create_generated_clock -name {SPI_Master_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 2 3} [list]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/hfclk}] -edges {1 1501 3001} [list]
create_generated_clock -name {SCB_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 2 3} [list]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/hfclk}] -edges {1 2 3} [list]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\ROUSSMA\Documents\PSoC Creator\RGB_OLED\RGB_OLED.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\ROUSSMA\Documents\PSoC Creator\RGB_OLED\RGB_OLED.cydsn\RGB_OLED.cyprj
# Date: Thu, 28 Dec 2017 15:28:33 GMT
