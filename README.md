<h1>The magicBoard!</h1>

The magicBoard is supposed to multi-purpose embedded device based on the MCU ARM-M7<br> 
STM32F767ZI, namelly on the development board [STM32 Nucleo-144](https://www.markdownguide.org/basic-syntax)
equipped with customized<br>shield to extend Nucleo's functionality.

<img src="/Docs/magicBoardPhoto.jpg" alt="Demo" title="Demo">

<h2>Intended usage is:</h2>
<ol>
  <li>WIFI/Ethernet controlled digital TFT Clock + FM Radio + Thermometer + Pager</li>
  <li>WIFI/Ethernet controlled water-your-plants device for remote/automatic control</li>
  <li>WIFI/Ethener/UART controlled experimental device to generate and process signals</li>
</ol>

<h2>STM32F767ZI Nucleo-144 main features:</h2>

<ol>
  <li>USB FS (VCOM, HID..)</li>
  <li>Ethernet (RJ45)</li>
  <li>8MHz and 32.768kHz XTAL</li>
  <li>Arduino and ZIO connectors - <b>interface for the magicBoard</b></li>
</ol>

<h2>magicBoard shield features:</h2>

For more hardware details see schematics located in the "Docs".

<ol>
  <li><b>MONO AUDIO IN - analog pre-processing of 3.5mm jack signal</b><ul>
      <li>add offset to an AC audio signal for DC signal for MCU's ADC sampling</li>
      <li>amplify the audio signal to use full Rail-to-Rail MCU's ADC sampling</li>
	  <li>allow AC audio as well as DC testing signal processing ('no AC coupling')</li>
	  <li>used OPAMP LMV358 as a weighted sum amplifier</li>
  </ul></li>
  <li><b>STEREO AUDIO OUT - analog amplification for a speaker</b><ul>
      <li>buffer/amplify MCU's DAC output to provide enough power for a speaker</li>
	  <li>allow 'no amplifier' or 'no AC coupling' mode to generate DC test waveforms</li>
	  <li>selectable maximal output voltage (3V3, 5V, VIN)</li>
	  <li>used OPAMP LM386 as an audio aplifier</li>
  </ul></li>
  <li><b>LCD - development and usage information display TFT 3.5''</b><ul>
      <li>Display text in fonts up to '100' for alarm clock usage</li>
	  <li>Display text of smaller formats for dense information</li>
	  <li>Parallel 8+5 communication bus</li>
  </ul></li>
  <li><b>2x PWM - power driver for DC-brush-motor</b><ul>
      <li>H-Bridge operation mode</li>
	  <li>Thermal and current shutdown</li>
	  <li>Experimental signal generation</li>
	  <li>selectable maximal output voltage (3V3, 5V, VIN)</li>
	  <li>used DRV8220 as a PWM driver</li>
	  <li>direct duty cycle control + sleep</li>
  </ul></li>
  <li><b>2x NRF24L01+ module interface</b><ul>
      <li>For low-power sensor interfacing</li>
	  <li>2x for on-board development (controlled by one host)</li>
	  <li>Power supply rail controlled by P-MOSFET</li>
	  <li>2-row female pin header (4x2)</li>
	  <li>SPI communication bus + IRQ, CE</li>
  </ul></li>
    <li><b>ESP8266-01 module interface</b><ul>
      <li>For WIFI applications </li>
	  <li>Power supply rail controlled by P-MOSFET</li>
	  <li>2-row female pin header (4x2)<\li>
	  <li>UART communication bus + IO, RST</li>
  </ul></li>
   <li><b>RDA5807M module interface</b><ul>
      <li>For FM Radio applications + RDS </li>
	  <li>Power supply rail controlled by P-MOSFET</li>
	  <li>male pin header (4x1)<\li>
	  <li>connection to AUDIO IN and AUDIO OUT possible</li>
	  <li>I2C communication bus</li>
  </ul></li>
    <li><b>25AA1024 EEPROM</b><ul>
      <li>Non-volatile memory to save MCU's FLASH</li>
	  <li>SPI communication bus</li>
  </ul></li>
      <li><b>4x Button</b><ul>
      <li>to emaluate all 4 keyboard arrows or the gaming W-A-S-D navigation</li>
  </ul></li>
  <li><b>2x LED</b><ul>
      <li>white and green for general purposes</li>
  </ul></li>
  <li><b>Power supply options</b><ul>
      <li>To use Nucleo-144 3V3, 5V or external voltages</li>
  </ul></li>
</ol>