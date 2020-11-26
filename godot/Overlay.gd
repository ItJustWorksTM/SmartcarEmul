extends Control

var buf: String = ""

func _input(event: InputEvent):
	if event.is_action_pressed("ui_accept"):
		if EmulGlue.is_board_available() and !$LineEdit.text.empty():
			EmulGlue.write_uart($LineEdit.text)
			$LineEdit.clear()

func _process(delta):
	if EmulGlue.is_board_available():
		$Label.text = EmulGlue.get_uart_buf_n(0)
