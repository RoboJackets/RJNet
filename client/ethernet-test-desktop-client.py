from tkinter import *
from tkinter import ttk
import socket
import threading

################## Backend

# Variables to replace bad text entry with previous valid value
validateOldOctet = ''
validateOldPort = ''


# Log backend
def writeToLog(type,msg):
	logText['state'] = 'normal'
	if logText.index('end-1c')!='1.0':
		logText.insert('end', '\n')
	logText.insert('end',str(str(int(float(logText.index('end - 1 line')))) + ' ')) # Insert line numbers at begining of lines
	# Adds tags for color highlighting
	if (type == 'ERROR'):
		logText.insert('end', 'Error: ')
		highlightSearch('Error: ','ERROR')
	elif (type == 'STATE'):
		logText.insert('end', 'State: ')
		highlightSearch('State: ','STATE')
	elif (type == 'SEND'):
		logText.insert('end', 'Send: ')
		highlightSearch('Send: ','SEND')
	elif (type == 'RECEIVE'):
		logText.insert('end', 'Receive: ')
		highlightSearch('Receive: ','RECEIVE')
	logText.insert('end', msg)
	currentAutoScrollLog = logAutoScrollState.get()
	if (currentAutoScrollLog):
		logText.see('end')
	logText['state'] = 'disabled'

def highlightSearch(keyword, tag):
    pos = '1.0'
    validPos = True
    while validPos:
        idx = logText.search(keyword, pos, END)
        if not idx:
            validPos = False
        else:
        	pos = '{}+{}c'.format(idx, len(keyword))
        	logText.tag_add(tag, idx, pos)

def clearLog(*arg):
	logText['state'] = 'normal'
	logText.delete('1.0','end')
	logText['state'] = 'disabled'


# IP and Port Entry Validation

def correctOctetEntry(var):
	currentOctet = var.get() #string of current octect box
	global validateOldOctet
	try:
		if(currentOctet != ''): 
			if(not str.isdigit(currentOctet)): # looks for number
				raise TypeError
			if (int(currentOctet) < -1 or int(currentOctet) > 255 or len(currentOctet) > 3): # 0 to 255 and less than 3 char
				raise ValueError
		validateOldOctet = currentOctet #passes checks
	except ValueError:
		writeToLog('ERROR','Not a valid value. The octet must be between 0 and 255.')
		if currentOctet != '' and int(currentOctet) > 255:
			validateOldOctet = '255'
		elif currentOctet != '' and int(currentOctet) < 0:
			validateOldOctet = '0'
		else:
			validateOldOctet = ''
		var.set(validateOldOctet)
	except TypeError:
		writeToLog('ERROR','Not a valid type. The octet must be a number.')
		var.set(validateOldOctet)


def correctPortEntry(var):
	currentPort = var.get() #string of current octect box
	global validateOldPort
	try:
		if(currentPort != ''): 
			if(not str.isdigit(currentPort)): # looks for number
				raise TypeError
			if (int(currentPort) < -1 or int(currentPort) > 65535 or len(currentPort) > 5): # Must be between 0 and 65535 and less than 5 characters
				raise ValueError
		validateOldPort = currentPort #passes checks
	except ValueError:
		writeToLog('ERROR','Not a valid value. The port must be between 0 and 65534.')
		if currentPort != '' and int(currentPort) > 65534:
			validateOldPort = '65534'
		elif currentPort != '' and int(currentPort) < 0:
			validateOldPort = '0'
		else:
			validateOldPort = ''
		var.set(validateOldPort)
	except TypeError:
		writeToLog('ERROR','Not a valid type. The port must be a number.')
		var.set(validateOldPort)


# Socket connection and packet validation
def sendServerPacket():
	def packetServerWrapper():
		writeToLog('STATE','Setting up socket...')	
		try:
			s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			# TODO custom timeout?
			timeoutDuration = 5
			s.settimeout(timeoutDuration)
			ECHO_SERVER_ADDRESS = checkAddress()
			ECHO_SERVER_PORT = checkPort()
			connectionAttemptMessage = 'Hosting connection at ' + ECHO_SERVER_ADDRESS + ' at port ' + str(ECHO_SERVER_PORT) + ' with a timeout of ' + str(timeoutDuration) + ' seconds.'
			writeToLog('STATE',connectionAttemptMessage)
			listenPacketButton['state'] = 'disabled'
			s.bind((ECHO_SERVER_ADDRESS, ECHO_SERVER_PORT))
			s.listen()
			acceptedConnection, acceptedAddress = s.accept()
			connectionSuccessMessage = 'Connected to ' + str(acceptedAddress[0]) + ' at port ' + str(acceptedAddress[1])
			writeToLog('STATE','Success!')
			writeToLog('STATE',connectionSuccessMessage)
			listenPacketButton.configure(text = 'Stop Packet', command=stopPacketSendLoop)
			listenPacketButton['state'] = 'normal'
			shouldLoop = True
			while(shouldLoop):
				receivedData = acceptedConnection.recv(1024).decode()
				writeToLog('RECEIVE', receivedData)
				packet = targetPacketValue.get()
				writeToLog('SEND',packet)
				acceptedConnection.sendall(bytes(packet,'utf-8'))
				shouldLoop = bool(loopPackets.get())
			writeToLog('STATE','Closing socket.')
			listenPacketButton.configure(text = 'Listen for Packet', command=sendServerPacket)
			s.close()
		except RuntimeError:
			pass # Message found in validation. Just to break as to not run connect
		except socket.timeout:
			listenPacketButton['state'] = 'normal'
			writeToLog('ERROR', 'Connection has timed out. Verify you are physically connected to a device with the entered IP address.')
		except socket.error as err: 
			listenPacketButton['state'] = 'normal'
			errorMessage = 'Socket creation failed with error ' + str(err) + '.'
			writeToLog('ERROR', errorMessage)
	t = threading.Thread(target=packetServerWrapper)
	t.start()

def sendClientPacket():
	def packetClientWrapper():
		writeToLog('STATE','Looking for socket...')	
		try:
			s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			# TODO custom timeout?
			timeoutDuration = 5
			s.settimeout(timeoutDuration)
			ECHO_CLIENT_ADDRESS = checkAddress()
			ECHO_CLIENT_PORT = checkPort()
			connectionAttemptMessage = 'Trying to connect to ' + ECHO_CLIENT_ADDRESS + ' at port ' + str(ECHO_CLIENT_PORT) + ' with a timeout of ' + str(timeoutDuration) + ' seconds.'
			writeToLog('STATE',connectionAttemptMessage)
			sendPacketButton['state'] = 'disabled'
			s.connect((ECHO_CLIENT_ADDRESS, ECHO_CLIENT_PORT))
			writeToLog('STATE','Success!')
			sendPacketButton.configure(text = 'Stop Packet', command=stopPacketSendLoop)
			sendPacketButton['state'] = 'normal'
			shouldLoop = True
			while(shouldLoop):
				packet = targetPacketValue.get()
				writeToLog('SEND',packet)
				s.sendall(bytes(packet,'utf-8'))
				receivedData = s.recv(1024).decode()
				writeToLog('RECEIVE', receivedData)
				shouldLoop = bool(loopPackets.get())
			writeToLog('STATE','Closing socket.')
			sendPacketButton.configure(text = 'Send Packet', command=sendClientPacket)
			s.close()
		except RuntimeError:
			pass # Message found in validation. Just to break as to not run connect
		except socket.timeout:
			sendPacketButton['state'] = 'normal'
			writeToLog('ERROR', 'Connection has timed out. Verify you are physically connected to a device with the entered IP address.')
		except socket.error as err: 
			sendPacketButton['state'] = 'normal'
			errorMessage = 'Socket creation failed with error ' + str(err) + '.'
			writeToLog('ERROR', errorMessage)
	t = threading.Thread(target=packetClientWrapper)
	t.start()

def stopPacketSendLoop():
	loopPackets.set(0)


def checkAddress():
	addressOctet1 = targetIpAddress1.get()
	addressOctet2 = targetIpAddress2.get()
	addressOctet3 = targetIpAddress3.get()
	addressOctet4 = targetIpAddress4.get()
	if(addressOctet1 == '' or addressOctet2 == '' or addressOctet3 == '' or addressOctet4 == ''): # Verifies none of the octets are missing. Rest of error handling is handled in entry 
		writeToLog('ERROR','Missing address octect.')
		raise RuntimeError
	fullAddress = addressOctet1 + '.' + addressOctet2 + '.' + addressOctet3 + '.' + addressOctet4
	return fullAddress


def checkPort():
	portValue = targetPortValue.get()
	if(portValue == ''):
		writeToLog('ERROR', 'Missing port.')
		raise RuntimeError
	return int(portValue)

################## GUI

root = Tk()
root.title('Ethernet Interface')

mainframe = ttk.Frame(root, padding='3 3 12 12')
mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
root.columnconfigure(0, weight=1)
root.rowconfigure(0, weight=1)

ipFrame = ttk.Frame(mainframe)
ipFrame.grid(column=2, row=1, sticky=(N, W, E, S))

logFrame = ttk.Frame(mainframe)
logFrame.grid(column=2, row=6, sticky=(N, W, E, S))

# Labels
ipLabel = ttk.Label(mainframe, text='Target IP:')
ipLabel.grid(column=1, row=1, sticky=W)

portLabel = ttk.Label(mainframe, text='Target Port:')
portLabel.grid(column=1, row=2, sticky=W)

packetLabel = ttk.Label(mainframe, text='Packet:')
packetLabel.grid(column=1, row=3, sticky=W)

# IP Entry
targetIpAddress1 = StringVar()
targetIpAddress1.trace('w', lambda nm, idx, mode, var=targetIpAddress1: correctOctetEntry(var))
ipEntryTextBox1 = ttk.Entry(ipFrame, width=3, textvariable=targetIpAddress1)
ipEntryTextBox1.insert(END, '192')
ipEntryTextBox1.grid(column=1, row=1, sticky=(W, E))

ipEntryDotLabel1 = ttk.Label(ipFrame, text='.')
ipEntryDotLabel1.grid(column=2, row=1, sticky=W)

targetIpAddress2 = StringVar()
targetIpAddress2.trace('w', lambda nm, idx, mode, var=targetIpAddress2: correctOctetEntry(var))
ipEntryTextBox2 = ttk.Entry(ipFrame, width=3, textvariable=targetIpAddress2)
ipEntryTextBox2.insert(END, '168')
ipEntryTextBox2.grid(column=3, row=1, sticky=(W, E))

ipEntryDotLabel2 = ttk.Label(ipFrame, text='.')
ipEntryDotLabel2.grid(column=4, row=1, sticky=W)

targetIpAddress3 = StringVar()
targetIpAddress3.trace('w', lambda nm, idx, mode, var=targetIpAddress3: correctOctetEntry(var))
ipEntryTextBox3 = ttk.Entry(ipFrame, width=3, textvariable=targetIpAddress3)
ipEntryTextBox3.insert(END, '1')
ipEntryTextBox3.grid(column=5, row=1, sticky=(W, E))

ipEntryDotLabel3 = ttk.Label(ipFrame, text='.')
ipEntryDotLabel3.grid(column=6, row=1, sticky=W)

targetIpAddress4 = StringVar()
targetIpAddress4.trace('w', lambda nm, idx, mode, var=targetIpAddress4: correctOctetEntry(var))
ipEntryTextBox4 = ttk.Entry(ipFrame, width=3, textvariable=targetIpAddress4)
ipEntryTextBox4.grid(column=7, row=1, sticky=(W, E))

# Port and Packet Entry
targetPortValue = StringVar()
targetPortValue.trace('w', lambda nm, idx, mode, var=targetPortValue: correctPortEntry(var))
portEntryTextBox = ttk.Entry(mainframe, width=5, textvariable=targetPortValue)
portEntryTextBox.grid(column=2, row=2, sticky=W)

targetPacketValue = StringVar()
packetEntryTextBox = ttk.Entry(mainframe, width=7, textvariable=targetPacketValue)
packetEntryTextBox.grid(column=2, row=3, sticky=(W, E))

# Loop Checkbox
loopPackets = IntVar()
loopPacketCheckBox = ttk.Checkbutton(mainframe, text='Loop', variable=loopPackets)
loopPacketCheckBox.grid(column=2, row=5, sticky=(W, E))

# Send Button
sendPacketButton = ttk.Button(mainframe, text='Send Packet', command=sendClientPacket)
sendPacketButton.grid(column=3, row=5, sticky=E)

# Client Button
listenPacketButton = ttk.Button(mainframe, text='Listen for Packet', command=sendServerPacket)
listenPacketButton.grid(column=4, row=5, sticky=E)

# Log

logLabel = ttk.Label(logFrame, text='Log:')
logLabel.grid(column=0, row=0, sticky=(N,W))

logScrollbar = ttk.Scrollbar(logFrame, orient=VERTICAL)
logScrollbar.grid(column=1, row=1, sticky=(W,N,S))

logText = Text(logFrame, state='disabled', width=80, height=24, wrap='word',yscrollcommand=logScrollbar.set)
logText.grid(column=0, row=1, sticky=(N, W, E, S))
logText.tag_config('ERROR', foreground='red') #Error tag
logText.tag_config('STATE', foreground='blue') #State tag
logText.tag_config('SEND', foreground='#cc0099') #Send tag
logText.tag_config('RECEIVE', foreground='#ff6600') #Receive tag
logScrollbar.config(command=logText.yview)

logAutoScrollState = IntVar(value = 1) #defaults check to on
logScrollCheckBox = ttk.Checkbutton(logFrame, text='Auto Scroll', variable=logAutoScrollState)
logScrollCheckBox.grid(column=0, row=2, sticky=(W, E))

logClearButton = ttk.Button(logFrame, text='Clear Log', command=clearLog)
logClearButton.grid(column=1, row=2, sticky=(E, S))




logFrame.pack()


for child in mainframe.winfo_children(): child.grid_configure(padx=5, pady=5)

ipEntryTextBox4.focus()
root.bind('<Return>', sendServerPacket)

root.mainloop()

