#! /usr/bin/ruby
# encoding: utf-8

require 'twitter'
require 'kconv'
require 'nkf'
require 'csv'

##
# BP3596 API
##
class BP3596PipeApi
  ##
  # func   : Read the data from the receiving pipe
  # input  : Receive pipe fp
  # return : Receive data
  ##
  def write_device(fp,data)
    ret = select(nil, [fp], nil, 0.1)
	begin
	  len = fp.write(data);
	rescue
	  raise EACK
	end
	return len;
  end
  def read_device(fp)
    # Data reception wait (timeout = 100ms)
    ret = select([fp], nil, nil, 0.1)

    # Reads the size of the received data
    len = fp.read(2)
    if ((len == "") || (len == nil)) then # read result is empty
      return -1
    end
    size =  len.unpack("S*")[0]

    # The received data is read
    recv_buf = fp.read(size)
    if ((recv_buf == "") || (recv_buf == nil)) then # read result is empty
      return -1
	end

    return recv_buf
  end
  def BinaryMonitor(raw)
    len = raw.length
    header = raw.unpack("H4")[0]

	# unsupported format
	if header != "21a8" then
	  unsupported_format(raw)
	  return
	end

	# supported format
    seq = raw[2].unpack("H2")[0]

	# PANID
    myPanid = raw[3..4].unpack("S*")[0]

	# RX Address
	rxAddr = raw[5..6].unpack("S*")[0]

	# TX Address
	txAddr = raw[7..8].unpack("S*")[0]

	# 
	print(sprintf("PANID=0x%04X, rxAddr=0x%04X, txAddr=0x%04X, DATA:: ",myPanid, rxAddr, txAddr))

	for num in 9..len-2 do
	  print(raw[num].unpack("H*")[0]," ")
    end
	print("\n") end

  # printing unsupported format
  def unsupported_format(raw)
    data = raw.unpack("H*")
	print("unsupported format::",data,"\n")
  end
end

##
# Main function
##
class MainFunction

  finish_flag = 0             # Finish flag                                                                              
# dstAddr=0x8B91
  dstAddr=0x9047
  srcAddr=0xAC54

  font = CSV.table('font.csv')

  client = Twitter::REST::Client.new do |config|
    config.consumer_key        = "hogehogehogehogehogehoge"
    config.consumer_secret     = "hogehogehogehogehogehoge"
    config.access_token        = "hogehogehogehogehogehoge"
    config.access_token_secret = "hogehogehogehogehogehoge"
  end

  while finish_flag==0 do
  	catch :retry_timeline do

  	# Process at the time of SIGINT Receiving
  	Signal.trap(:INT){
    finish_flag=1
  	}

	# 特定ユーザのtimeline取得
	#@client.user_timeline("LazuriteBot").each do |timeline|
	  #puts @client.status(timeline.id).text
	#end

	# 特定ユーザのtimelineを件数(1件)指定して取得
	  client.user_timeline("nhk_news", { count: 1 } ).each do |timeline|
	    $message = client.status(timeline.id).text
		if $tweetTime == timeline.created_at then
        	sleep(60)
			throw :retry_timeline 
		else
			$tweetTime = timeline.created_at
    		p $tweetTime
		end
	  end
	  $message = $message.tr('0-9a-zA-Z','０-９ａ-ｚＡ-Ｚ')

	  print($message,"\n")
	  list_char = $message.split("")
	  length = 0
	  font_raw = []

	  for letter in list_char do
  	  	letter_sjis = letter.tosjis.unpack('S*')
  	  	code_sjis = letter_sjis[0]
  	  	if letter_sjis[0].nil? then
  	  	else
	      i = code_sjis/256 + (code_sjis%256)*256
   	      k = font[:code].index(i)
   	   	  print(sprintf("%s %04x %02x %02x %02x %02x %02x %02x %02x %02x\n",letter, font[:code][k], font[:d0][k],font[:d1][k],font[:d2][k],font[:d3][k], font[:d4][k],font[:d5][k],font[:d6][k],font[:d7][k]))
   	      font_raw.push(font[:d0][k],font[:d1][k],font[:d2][k],font[:d3][k], font[:d4][k],font[:d5][k],font[:d6][k],font[:d7][k])
      	  length = length + 8
      	end
  	  end

	  #  k = font[:code].index(j)
	  #  p font[k]
	  #  print(sprintf("0x%04x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",font[:code][k], font[:d0][k],font[:d1][k],font[:d2][k],font[:d3][k], font[:d4][k],font[:d5][k],font[:d6][k],font[:d7][k]))
	  font_raw.insert(0,length%256)
	  font_raw.insert(1,length/256)
	  for i in font_raw.count..2048 do
	      font_raw.push(0);
	  end
  
      value = %x(sudo insmod /home/pi/driver/sub-ghz/DRV_802154.ko ch=36)
	  
	  ### Variable definition
	  bp3596_dev  = "/dev/bp3596" # Receiving pipe

	  # Open the Receiving pipe
	  bp3596_fp = open(bp3596_dev, "rb")

	  bp_api = BP3596PipeApi.new

	  # Loop until it receives a SIGINT
	  while 1
  		# Read device
 	   	recv_buf = bp_api.read_device(bp3596_fp)
	    if recv_buf == -1
   	 		next
 		end

		if /Twitter Beacon/ =~ recv_buf
    		print(recv_buf)
			break
		end
  		# Create a transmit buffer from the receive buffer
    	print(recv_buf)
 	  end
  	  # Close the Receiving pipe
  	  bp3596_fp.close


	  # Open the Receiving pipe
	  bp3596_wr = open(bp3596_dev, "wb")

	  # data = [0xa821,0x00,0xabcd,0x8e35,0x87a4,"Flash"].pack("scsssa5")
	  # mac_header = [0xa821,0x00,0xabcd,0x9047,0x87a4]
	  mac_header = [0xa821,0x00,0xabcd,dstAddr,srcAddr]
	  for i in 0..15 do
 		send_raw = Array.new()
   	   	flash_write=sprintf("FlashWrite%x",i)
   	   	send_raw.concat(mac_header)
        send_raw.push(flash_write)
        send_raw.concat(font_raw[i*128..(i+1)*128-1])
        p send_raw
        send_raw_bin = send_raw.pack("SCSSSA12C*")
	   #retry_limitter = 0
		catch :send_success do 
			for j in 1..5 do
	       		begin
              	#	print("send phy request\n")
         		    status = bp_api.write_device(bp3596_wr,send_raw_bin)
       	       		p status
					throw :send_success
       			rescue
        			print("does not receive ack\n")
	   	#		retry_limitter += 1
	   	#		if retry_limitter < 5 then
	   	#			redo
	   	#		end
        		end
        		sleep(0.1)
			end
 		end
   	  end
  	  bp3596_wr.close
	  value = %x(sudo rmmod DRV_802154)
    end # catch
  end
end
