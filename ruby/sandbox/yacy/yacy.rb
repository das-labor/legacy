require 'rubygems'
require 'base64'

class Hash;def /(key);self[key];end;end

def rebuild_hash(str);
  h={};str.gsub(/[{}]/,'').split(',').each{|i|i=i.split('=');h[i[0]]=i[1];};h
end

def read_seedtxt(file)
  seeds={};File.open(file).readlines.each do |line|
    hash = rebuild_hash Base64.decode64(line.gsub(/b\|/,'').chop)
    seeds[hash['Hash']] = hash if hash
  end;return seeds
end

@seeds = read_seedtxt('seed.txt')


p @seeds.size
@seeds['H0JEGZQg1q54'].each { |i| p i }