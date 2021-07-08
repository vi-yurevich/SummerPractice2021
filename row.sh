#!/bin/bash
g++ -std=c++17 main.cpp -o search.out

if [[ $1 = "-c" ]]; then
	shift
	if [[ $# -eq 1 ]] ; then
		s=C
	else
		s=$2
	fi
	if [[ "$1" == *","* ]] ; then
		p=$1
		p=`echo $p | tr ',' ' '`
		./search.out 1 $p > row.t
		cat row.t
	else 
		echo -e "Паттерн должен содержать хотя бы два интервала"
		exit 1
	fi
	

elif [[ $1 = "-pr" ]] ; then
	if [[ -r row.t ]] ; then
		cat row.t
	else 
		echo -e "Файл row.t не существует или недоступен для чтения"
		exit 1
	fi
elif [[ $1 = "-pl" ]] ; then
	if [[ $# -lt 2 ]] ; then
		echo -e "Для проигрывания серии в качестве параметра необходимо указать номер серии в файле row.t"
		exit 1
	elif [[ !( -r row.t ) ]] ; then
		echo -e "Файл row.t не существует или недоступен для чтения"
		exit 1
	fi
	shift
	number="${1}"

	if [[ "$number" = "0"* ]]; then
		echo "Номер серии необходимо указывать без непозиционных нулей"
		exit 1
	fi
	key1=1
	key2=1
	[[ $number -lt 100 ]] && key1=0
	[[ $number -lt 10 ]] && key2=0

	if [[ key1 -eq 0 ]] && [[ key2 -eq 1 ]] ; then
		number=`echo "0${number}"`
	elif [[ key1 -eq 0 ]] && [[ key2 -eq 0 ]] ; then
		number=`echo "00${number}"`	
	fi

	if [[ ")" != "$number"* ]] ; then
		number=`echo "${number})"`
	fi

	row=`grep $number row.t`
	if [[ "$row" = "$number"* ]] ; then 
		row=`echo ${row//$number}`
		row=`echo $row | tr ',' ' '` 
		row=`echo ${row//[}`
		row=`echo ${row//]}`
		row=`echo ${row//)}`
		r=$row
		r=`echo $r | tr -d [:digit:] | tr -d -`
		echo -e "\nВопроизведение серии: $r\n"
		python3 row.py -p $row
	else
		echo "Серия с указанным номером отсутсвует в файле"
	fi


elif [[ $1 = "-md" ]] ; then
	if [[ $# -lt 2 ]] ; then
		echo -e "Для проигрывания серии в качестве параметра необходимо указать номер серии в файле row.t"
		exit 1
	elif [[ !( -r row.t ) ]] ; then
		echo -e "Файл row.t не существует или недоступен для чтения"
		exit 1
	fi
	shift
	number="${1}"

	if [[ "$number" = "0"* ]]; then
		echo "Номер серии необходимо указывать без непозиционных нулей"
		exit 1
	fi
	key1=1
	key2=1
	[[ $number -lt 100 ]] && key1=0
	[[ $number -lt 10 ]] && key2=0

	if [[ key1 -eq 0 ]] && [[ key2 -eq 1 ]] ; then
		number=`echo "0${number}"`
	elif [[ key1 -eq 0 ]] && [[ key2 -eq 0 ]] ; then
		number=`echo "00${number}"`	
	fi

	if [[ ")" != "$number"* ]] ; then
		number=`echo "${number})"`
	fi

	row=`grep $number row.t`
	if [[ "$row" = "$number"* ]] ; then 
		row=`echo ${row//$number}`
		row=`echo $row | tr ',' ' '` 
		row=`echo ${row//[}`
		row=`echo ${row//]}`
		row=`echo ${row//)}`
		r=$row
		r=`echo $r | tr -d [:digit:] | tr -d -`
		python3 row.py -m $row
		echo "MIDI файл с серией успешно создан"
	else
		echo "Серия с указанным номером отсутсвует в файле"
	fi
elif [[ $1 = "-mtrx" ]]; then
	if [[ $# -lt 2 ]] ; then
		echo -e "Для вычисления серийной матрицы в качестве параметра необходимо указать номер серии в файле row.t"
		exit 1
	elif [[ !( -r row.t ) ]] ; then
		echo -e "Файл row.t не существует или недоступен для чтения"
		exit 1
	fi
	shift
	number="${1}"

	if [[ "$number" = "0"* ]]; then
		echo "Номер серии необходимо указывать без непозиционных нулей"
		exit 1
	fi
	key1=1
	key2=1
	[[ $number -lt 100 ]] && key1=0
	[[ $number -lt 10 ]] && key2=0

	if [[ key1 -eq 0 ]] && [[ key2 -eq 1 ]] ; then
		number=`echo "0${number}"`
	elif [[ key1 -eq 0 ]] && [[ key2 -eq 0 ]] ; then
		number=`echo "00${number}"`	
	fi

	if [[ ")" != "$number"* ]] ; then
		number=`echo "${number})"`
	fi

	row=`grep $number row.t`
	if [[ "$row" = "$number"* ]] ; then 
		row=`echo ${row//$number}`
		row=`echo $row | tr ',' ' '` 
		row=`echo ${row//[}`
		row=`echo ${row//]}`
		row=`echo ${row//)}`
		row=`echo $row | tr -d [:digit:] | tr -d -`
		./search.out 2 $row > row_mtrx.t
		cat row_mtrx.t
	else
		echo "Серия с указанным номером отсутсвует в файле"
	fi	

elif [[ $1 = "-g" ]] ; then
	if [[ $# -lt 2 ]] ; then
		echo -e "Для построения графа серии в качестве параметра необходимо указать номер серии в файле row.t"
		exit 1
	elif [[ !( -r row.t ) ]] ; then
		echo -e "Файл row.t не существует или недоступен для чтения"
		exit 1
	fi
	shift
	number="${1}"

	if [[ "$number" = "0"* ]]; then
		echo "Номер серии необходимо указывать без непозиционных нулей"
		exit 1
	fi
	key1=1
	key2=1
	[[ $number -lt 100 ]] && key1=0
	[[ $number -lt 10 ]] && key2=0

	if [[ key1 -eq 0 ]] && [[ key2 -eq 1 ]] ; then
		number=`echo "0${number}"`
	elif [[ key1 -eq 0 ]] && [[ key2 -eq 0 ]] ; then
		number=`echo "00${number}"`	
	fi

	if [[ ")" != "$number"* ]] ; then
		number=`echo "${number})"`
	fi

	row=`grep $number row.t`
	if [[ "$row" = "$number"* ]] ; then
		row=`echo $row | tr -d [:digit:] | tr -d -`
		row=`echo ${row//)}`
		row=`echo ${row/[/}`
		row=`echo ${row/]/;}`
		for (( i=1; i <= 12; ++i)) ; do
			row=`echo ${row/,/->}`
		done
		row=`echo ${row/F#/\"F#\"}`
		touch g.gv
		echo -e "digraph g1 {\n\tnode [shape = circle];\n\tedge [color="white"];" >> g.gv
		echo -e '\t"C" -> "Db" -> "D" -> "Eb" -> "E" -> "F";' >> g.gv
		echo -e '\t"F"-> "F#" -> "G" -> "Ab" -> "A" -> "Bb" -> "B" -> "C";\n}' >> g.gv
		circo g.gv > tmp.gv
		tr -d '}' < tmp.gv > tmp_g.gv
		echo -e "\tedge [color="black"];\n\t$row\n}" >> tmp_g.gv
		neato -n tmp_g.gv -Tpng -orow.png
		shotwell "row.png"
		rm g.gv tmp.gv tmp_g.gv
	else
		echo "Серия с указанным номером отсутсвует в файле"
	fi

elif [[ $1 = "-rmv" ]] ; then
	rm search.out row.t	row_mtrx.t row.mid row.png

elif [[ $1 = "-h" ]] ; then
	echo -e "-c  [интревальный паттерн через запятую без пробелов]"
	echo -e "построение додекафонических серий с помощью введёного паттерна и его преобразований. "
	echo -e "Результат записывается в файл row.t\n"

	echo -e "-pr\nвывод на экран содержимого файла row.t\n" 

	echo -e "-pl  [номер серии из row.t без непозиционных нулей]\nроигрывание указанной серии\n"

	echo -e "-md [номер серии из row.t без непозиционных нулей]\создание midi файла указанной серии\n"


	echo -e "-mtrx [номер серии из row.t  без непозиционных нулей]\nпостроение серийной матрицы, запись её в файл row_mtrx.t, вывод на экран\n"

	echo -e "-g [номер серии из row.t  без непозиционных нулей]\nпостроение графа серии\n"
	
	echo -e "-h\nвывод руководства по использованию\n"

elif [[ $# -eq 1 ]] ; then
	echo "Необходимо указать один ключ. Для вывода руководства используйте ключ -h"
else
	echo "Неверный ключ"

fi

#cut -d'|' -f13 row_mtrx.t
#circo graph.gv > tempgraph.gv
#neato -n tempgraph.gv -Tpng -O
