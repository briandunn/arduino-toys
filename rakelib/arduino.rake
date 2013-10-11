root = Pathname(__FILE__).dirname.join '..'
build_path = root.join 'tmp/arduino'
arduino_path = Pathname '/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino'
core_path = arduino_path.join 'cores/arduino'
dot_o = ->(source) { build_path.join "#{source.basename}.o" }
directory build_path.to_s
$arduino_lib = build_path.join 'core.a'

CXX_FLAGS = <<-FLAGS.gsub /\s+/, ' '
  -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections
  -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null
  -DARDUINO=105 -I#{arduino_path.join 'cores/arduino'}
  -I#{arduino_path.join 'variants/standard'}
FLAGS

core_sources = %w[WInterrupts.c wiring.c wiring_analog.c wiring_digital.c
  wiring_pulse.c wiring_shift.c CDC.cpp HardwareSerial.cpp HID.cpp IPAddress.cpp
  main.cpp new.cpp Print.cpp Stream.cpp Tone.cpp USBCore.cpp WMath.cpp
  WString.cpp avr-libc/malloc.c avr-libc/realloc.c].map &core_path.method(:join)

for source in core_sources
  file dot_o[source] => [source, build_path] do |t|
    sh "avr-gcc #{CXX_FLAGS} -c #{t.prerequisites.first} -o #{t.name}"
  end
end

file $arduino_lib => core_sources.map(&dot_o) do |t|
  sh "avr-ar rcs #{t.name} #{ t.prerequisites.join ' ' }"
end

namespace :arduino do
  desc 'builds arduino core library'
  task build: $arduino_lib

  desc 'cleans the arduino core library'
  task clean: :build do
    FileUtils.rm_rf build_path
  end
end

task arduino: 'arduino:build'
