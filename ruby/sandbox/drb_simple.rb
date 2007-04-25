require 'drb'

# DRb SERVER
class TestServer
  def doit(w)
    "#{w}"
  end
  def do
    "Hello, Distributed World"
  end
end

aServerObject = TestServer.new
DRb.start_service('druby://localhost:9000', aServerObject)
DRb.thread.join # Don't exit just yet!


# DRb CLIENT
DRb.start_service()
obj = DRbObject.new(nil, 'druby://localhost:9000')