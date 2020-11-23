extends Spatial

func _input(event):
	if Input.is_action_just_pressed("ui_accept"):
		if EmulGlue.start_compile("/home/ruthgerd/test.ino"):
			var result = yield(EmulGlue, "compile_done")
			print("Yielded from godot!")
			print(result)
		else:
			print("straight rejection")

