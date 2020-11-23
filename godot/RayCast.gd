extends RayCast


func _physics_process(delta):
	if is_colliding():
		print("dist: " + str(global_transform.origin.distance_to(get_collider().global_transform.origin)))
