cmd_/home/pi/lessen/Opdracht6/modules.order := {   echo /home/pi/lessen/Opdracht6/opdracht6.ko; :; } | awk '!x[$$0]++' - > /home/pi/lessen/Opdracht6/modules.order
