# Tarea 1
Integrantes: Manuel Silva, Iván Wolf

## Sobre la simulación

Simulamos segundo a segundo el funcionamiento del scheduler. Para la cola de los procesos en estado `RUNNING` utilizamos un max heap. El programa también mantiene una lista de todos los procesos para revisar en todos los tiempos si es que es necesario despertar a un proceso. 
Cuando un proceso termina en el tiempo `t` el scheduler ejecuta el próximo programa en el tiempo `t + 1`.
