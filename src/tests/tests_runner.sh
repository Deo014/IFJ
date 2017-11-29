#! /bin/sh

proj=./proj
inputs_succ_error0=./tests/vstupy/succ_error0/*
inputs_lexx_error1=./tests/vstupy/lexx_error1/*
inputs_synt_error2=./tests/vstupy/synt_error2/*
inputs_semm_error3=./tests/vstupy/semm_error3/*
inputs_semm_error4=./tests/vstupy/semm_error4/*
i=0 # counter

echo "----------SUCCESS ERROR TESTS 0----------"
# testy na lexikalni chyby
for f in $inputs_succ_error0
do
    test_name=$(basename $f)

	test_output=$(cat $f | ./$proj) # test output
	return_value=$? # test result code
	i=$(($i + 1)) # test number

    # VYPSANI TESTU
	printf "%-02d  " "$i"
	if [ "$return_value" -eq "0" ]; then
	    printf "   OK:  "
	else
	    printf "ERROR:  "
	fi
	printf "%-50s EXPECTED=0, RETURNED=$return_value\n" "$test_name"
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
	    printf "   OK:  "
	else
	    printf "ERROR:  "
	fi
	printf "%-50s EXPECTED=1, RETURNED=$return_value\n" "$test_name"
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
	    printf "   OK:  "
	else
	    printf "ERROR:  "
	fi
	printf "%-50s EXPECTED=2, RETURNED=$return_value\n" "$test_name"

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
	    printf "   OK:  "
	else
	    printf "ERROR:  "
	fi
	printf "%-50s EXPECTED=3, RETURNED=$return_value\n" "$test_name"

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
	    printf "   OK:  "
	else
	    printf "ERROR:  "
	fi
	printf "%-50s EXPECTED=4, RETURNED=$return_value\n" "$test_name"

done

