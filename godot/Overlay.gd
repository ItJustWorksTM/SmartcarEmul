extends Control

var buf: String = ""
onready var input: LineEdit = $Panel/LineEdit
onready var label: Label = $Panel/Label

func _input(event: InputEvent):
	if event.is_action_pressed("ui_accept"):
		if EmulGlue.is_board_available() and !input.text.empty():
			EmulGlue.write_uart(input.text)
			input.clear()

func _process(delta):
	if EmulGlue.is_board_available():
		label.text = EmulGlue.get_uart_buf_n(0)


func _on_Button_pressed():
	if EmulGlue.start_compile("/home/ruthgerd/test.ino"):
		var result = yield(EmulGlue, "compile_done")
		print(result)
