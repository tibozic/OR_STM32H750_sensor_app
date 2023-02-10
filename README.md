# OR DN2
Za poganjanje se mapo OR\_2DN\_STM32H7\_app uvozi v CubeIDE, spremeniti pa je potrebno
pot do header datotek \v{c}ipa, to se nahaja pod Project->Properties->C/C++ General->Paths
and Symbols. Tukaj dodamo 2 novi poti do OR_STM32H750_sensor_app/chip_header/CMSIS/Include
in OR_STM32H750_sensor_app/chip_header/CMSIS/Device/ST/STM32H7xx/Include.
