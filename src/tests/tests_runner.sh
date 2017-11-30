#! /bin/sh

proj=./proj
interpreter=./ic17int
test_output_file=./tests/tmp_file

inputs_succ_error0=./tests/vstupy/succ_error0/*.input
outputs_succ_error0_path=./tests/vstupy/succ_error0
inputs_lexx_error1=./tests/vstupy/lexx_error1/*.input
inputs_synt_error2=./tests/vstupy/synt_error2/*.input
inputs_semm_error3=./tests/vstupy/semm_error3/*.input
inputs_semm_error4=./tests/vstupy/semm_error4/*.input
i=0 # counter

echo "----------SUCCESS ERROR TESTS 0----------"
# testy na lexikalni chyby
for f in $inputs_succ_error0
do
	# PROMENNE
	filename=$(basename "$f")
	test_name="${filename%.*}"
	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number
    
	# POKUD BYL VRACEN CODE 0: VYHODNOCENI INTSTRUKCI INTERPRETEREM
	if [ $return_value -eq 0 ]; then
		printf "$test_output" > $test_output_file # vypsani ifjcode17 do tmp souboru
		./$interpreter $test_output_file > $test_output_file 2>&1 # vypsani interpreter output do tmp souboru
		interpreter_result=$(diff $test_output_file $outputs_succ_error0_path/$test_name.output)
	fi

    # VYPSANI TESTU
	printf "%-02d  " "$i" # vypis cisla testu
	if [ "$return_value" -eq "0" ] && [ -z "$interpreter_result" ]; then # vypis OK/ERROR
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi

	printf "%s.input | EXPECTED=0, RETURNED=$return_value" "$test_name" # vypis ERROR CODE
	
	if [ $return_value -eq 0 ]; then # vypis INTERPRETU
		if [ -z "$interpreter_result" ]; then
			printf " INTERPRETER OUTPUT OK"
		else
			printf " INTERPRETER OUTPUT DIFFER\n"
			cat $test_output_file
		fi
	fi
	printf "\n"
done

echo "----------LEXXICAL ERROR TESTS 1----------"
# testy na lexikalni chyby
for f in $inputs_lexx_error1
do
    test_name=$(basename $f)

	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number

    # VYPSANI TESTU
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "1" ]; then
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi
	printf "%-8s | EXPECTED=1, RETURNED=$return_value\n" "$test_name"
done

echo "----------SYNTAX ERROR TESTS 2----------"
# testy na syntakticke chybu
for f in $inputs_synt_error2
do
    test_name=$(basename $f)

	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number

    # VYPSANI TESTU
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "2" ]; then
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi
	printf "%-8s | EXPECTED=2, RETURNED=$return_value\n" "$test_name"

done

echo "----------SEMANTIC ERROR TESTS 3----------"
# testy na syntakticke chybu
for f in $inputs_semm_error3
do
    test_name=$(basename $f)

	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number

    # VYPSANI TESTU
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "3" ]; then
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi
	printf "%-8s | EXPECTED=3, RETURNED=$return_value\n" "$test_name"

done

echo "----------SEMANTIC ERROR TESTS 4----------"
# testy na syntakticke chybu
for f in $inputs_semm_error4
do
    test_name=$(basename $f)

	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number

    # VYPSANI TESTU
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "4" ]; then
	    printf "   OK   "
	else
	    printf "ERROR   "
	fi
	printf "%-8s | EXPECTED=4, RETURNED=$return_value\n" "$test_name"

done

