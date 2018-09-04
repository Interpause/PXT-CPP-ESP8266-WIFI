let item = 0
radio.onDataPacketReceived( ({ receivedString: name, receivedNumber: value }) =>  {
    basic.showString("Hello!")
})
basic.showLeds(`
    . . . . .
    . . . . .
    . . . . .
    . . . . .
    . . . . .
    `)
basic.showIcon(IconNames.Heart)
basic.showString("Hello!")
item = 0
basic.forever(() => {
	
})
