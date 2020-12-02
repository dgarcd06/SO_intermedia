#!/bin/bash

ls practicaIntermedia.c

function compilar()
{
	if(-f $practicaIntermedia.c)
	then
		gcc practicaIntermedia.c -o practicaIntermedia
		echo -e "\nPrograma compilado correctamente"
	else
		echo -e "\nNo se ha podido encontrar el programa a compilar" 
	fi
}
function mostrar_codigo()
{
	if(-f $practicaIntermedia.c)
	then
		cat $practicaIntermedia
	else
		echo -e "\nNo se ha podido encontrar el programa" 
	fi

}

function ejecutar()
{
	if(-f $practicaIntermedia)
	then
		./practicaIntermedia || echo -e "\nNo tienes permisos para ejecutar el archivo"
	else
		echo -e "El programa no ha sido compilado previamente"
	fi
}

while true
do

	echo -e "\n Seleccione una de las siguientes opciones: "
	echo -e "\n 1- Mostrar el codigo del programa"
	echo -e "\n 2- Compilar archivo"
	echo -e "\n 3- Ejecutar programa(debe haber sido compilado antes)"
	echo -e "\n 4- Salir"

	read option

	case $option in

	1) mostrar_codigo
		;;

	2) compilar
		;;

	3) ejecutar
		;;

	4) break
		;;

	*)echo "Error: ha introducido un numero incorrecto"
		;;

	esac


done
exit 0
