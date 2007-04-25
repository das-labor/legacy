class UnameCommand
  def get_data
    %x(uname -a)
  end
end


class XmlReport
  def self.parse(data)
    arr = data.split
    XmlReport.new(arr[0], arr[1], arr[2])
  end
  
  def initialize(os_name, host_name, kernel_version)
    @os_name = os_name
  @host_name = host_name
  @kernel_version = kernel_version
  end

  def to_xml
    <<END
<report>
  <os>#{@os_name}</os>
  <host>#{@host_name}</host>
  <kernel>#{@kernel_version}</kernel>
</report>
END
  end
end

puts XmlReport.parse(UnameCommand.new.get_data).to_xml