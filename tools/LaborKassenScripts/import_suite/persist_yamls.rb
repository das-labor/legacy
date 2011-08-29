require "#{File.dirname __FILE__}/common"

unless ARGV.length == 1
  $stderr << "Usage: #{$0} <path to yaml file>\n"
  $stderr << "Imports records in yaml file into database, marking them as imported into gnucash\n"
  exit 1
end

setup_db

yamls = `cat #{ARGV[0]}`
abort("Could not open file, cat exited with status #{$?.exitstatus}") unless $?.exitstatus.zero?

transactions = []
YAML::load_documents(yamls) { |doc| transactions << Transaction.new(doc) }

transactions.each { |tran| tran.save }
