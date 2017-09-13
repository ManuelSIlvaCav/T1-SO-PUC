# Tarea2
Integrantes:
Manuel Silva 13637207
Iván Wolf xxxxxxx


msh
Para esta parte se utilizaron los siguientes supuestos.
1) Como no se especifica los &n procesos si estos eran en background o en primer plano, se considero el segundo caso... por lo tanto no es posible ingresar comandos cuando se estan ejecutando los n procesos.

2) Para setear el path se sigue como en el enunciado setPath home/name/Desktop/ respetando las mayusculas de setpath (lo mismo para setPrompt)

3) Si se setea el path por ejemplo "setPath /bin/" y se trata de ejecutar "/usr/bin/time" no se podra ya que se estaria usando mal la finalidad del path... por eso en caso de querer revertir la asigacion, se creo un builtin "resetPath" sin argumentos que restaura el path original.

4.a) No se especifica como es el ccomportamiento en caso e Ctrl+C cuando hay procesos background y se esta esperando comandos... en este caso se termina de igual manera la ejecucion de msh, pero se deja ejecutando el procesos que estaba en background.

4.b) De igual manera para los N procesos en paralelo, como estos se asumio que corrian en primer plano (no background) se termina la ejecucion de todos (SIGKILL >:) ) y se retorna a msh para recibir mas comandos.

5) Para setPrompt con * se reemplaza el exitcode del ultimo proceso donde este el * dentro del string.
