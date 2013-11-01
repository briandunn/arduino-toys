root = Pathname(__FILE__).dirname.join '..'
build_path = root.join 'tmp/arduino'
$arduino_path = Pathname '/Applications/Arduino.app/Contents/Resources/Java/hardware'
core_path = $arduino_path.join 'arduino/cores/arduino'
directory build_path.to_s
arduino_lib = build_path.join 'core.a'
bin_path = $arduino_path.join 'tools/avr/bin/'
$gcc = bin_path.join 'avr-gcc'
ar = bin_path.join 'avr-ar'
objcopy = bin_path.join 'avr-objcopy'
avrdude = bin_path.join 'avrdude'

($CXX_FLAGS ||= '') << <<-FLAGS.gsub(/\s+/, ' ')
  -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections
  -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null
  -DARDUINO=105 -I#{core_path}
  -I#{$arduino_path.join 'arduino/variants/standard'}
FLAGS

core_sources = FileList[*%w[WInterrupts.c wiring.c wiring_analog.c wiring_digital.c
  wiring_pulse.c wiring_shift.c CDC.cpp HardwareSerial.cpp HID.cpp IPAddress.cpp
  main.cpp new.cpp Print.cpp Stream.cpp Tone.cpp USBCore.cpp WMath.cpp
  WString.cpp avr-libc/malloc.c avr-libc/realloc.c].map(&core_path.method(:join)).map(&:to_s)]

for source in core_sources
  file source.pathmap("#{build_path}/%n.o") => [source, build_path] do |t|
    sh "#$gcc #$CXX_FLAGS -c #{t.prerequisites.first} -o #{t.name}"
  end
end

file arduino_lib => core_sources.pathmap("#{build_path}/%n.o") do |t|
  sh "#{ar} rcs #{t.name} #{ t.prerequisites.join ' ' }"
end

namespace :arduino do
  desc 'builds arduino core library'
  task build: arduino_lib

  desc 'cleans the arduino core library'
  task clean: :build do
    FileUtils.rm_rf build_path
  end
end

task arduino: 'arduino:build'

file 'main.hex' => arduino_lib do |t|
  sh "#$gcc -Os -Wl,--gc-sections -L. -lm -mmcu=atmega328p -o main.elf #{t.prerequisites.join ' '}"
  sh "#{objcopy} -O ihex -R .eeprom main.elf #{t.name}"
end

task build: 'main.hex'

task upload: :build do |t|
  config = $arduino_path.join 'tools/avr/etc/avrdude.conf'
  port = Dir['/dev/tty.usbmodem*'].first or raise "plug the arduino in!"
  sh "#{avrdude} -C #{config} -v -v -pm328p -carduino -P #{port} -U flash:w:main.hex:i"
end
