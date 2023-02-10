# OR DN2
Za poganjanje se mapo OR_2DN_STM32H7_app uvozi v CubeIDE, spremeniti pa je potrebno
pot do header datotek čipa, to se nahaja pod Project->Properties->C/C++ General->Paths
and Symbols. Tukaj dodamo 2 novi poti do OR_STM32H750_sensor_app/chip_header/CMSIS/Include
in OR_STM32H750_sensor_app/chip_header/CMSIS/Device/ST/STM32H7xx/Include.
