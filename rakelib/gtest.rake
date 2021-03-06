root = Pathname(__FILE__).dirname.join '..'
gtest_path = root.join 'gtest'
build_path = root.join 'tmp/gtest'
gtest_lib = build_path.join 'gtest.a'
gtest_sources = %w[-all _main].map do |source|
  gtest_path.join "src/gtest#{source}.cc"
end
dot_o = ->(source) { build_path.join "#{source.basename}.o" }

directory build_path.to_s

gtest_sources.each do |source|
  file dot_o[source] => [source, build_path] do |t|
    sh <<-SH.gsub /\s+/, ' '
      c++ -c -I #{gtest_path}
             -I #{gtest_path.join 'include'}
             #{source} -o #{t.name}
    SH
  end
end

file gtest_lib => gtest_sources.map(&dot_o) do |t|
  sh "ar -rv #{t.name} #{t.prerequisites.join ' '}"
end

namespace :gtest do
  desc 'builds the gtest library'
  task build: gtest_lib

  desc 'cleans the gtest library'
  task clean: :build do
    FileUtils.rm_rf build_path
  end
end
task gtest: 'gtest:build'

task 'test/run' => gtest_lib.expand_path

task test: 'test/run' do
  sh 'test/run'
end
