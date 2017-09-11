#!/bin/bash

# Name command_line.sh
# Date 10/05/2017

# Project

# Description
#
# Test based on Command line.
# Totally based on Linux commands.
#



#########        START of -          function func_signin__take_test()          ##########
function func_signin__take_test()
{

	question_number=1
	number_of_questions=10

	user=$1													# Signed-in user
	user_answer_file=${user}_answers.txt					# Create answers file for signed-in user
	touch $user_answer_file
	echo -e "Question Number\t\tAnswer Selected\t\tCorrect Option\t\tAnswer\t\tTime in Sec" > $user_answer_file



	############################### START of - To generate and store random numbers in an array ##########################

	for((index_random=1;index_random<=$number_of_questions;index_random++))     # For loop to generate 20% random numbers
	do

		let random_number=$RANDOM%$number_of_questions+1     # Generate random number within the given range of total lines

		# To remove duplicate random numbers, each new random number generated is compared with elements of array containing random numbers
		array_random_number[$index_random]=$random_number
		
		for((index_array=1;index_array<$index_random;index_array++))
		do  
			if [ ${array_random_number[$index_array]} -eq $random_number ]
			then
				let index_random=$index_random-1    # If new random number already exists, then index_random is not moved to next location
				break
			fi  
		done
	done


	################################## END of - To generate and store random numbers in an array #########################



	################################## START of - To pick ques from ques bank and print ques #############################

	marks=0
	
	for((question_number=1;question_number<=number_of_questions;question_number++))
	do	

		clear															# To clear screen

		random_question_number=${array_random_number[$question_number]}	# Random number fetched from array

		echo Question from Question bank :
		echo

		echo Question $question_number')'										# Display "Question Number"

		question_number_format=$random_question_number					# Question number is in format eg. 5) in question_bank.txt

		sed -n '/'$question_number_format')/,/^$/p' question_bank.txt	# Random question number selected from file question_bank.txt

		echo -e "\nSelect option a, b, c or d . Time alloted = 10 Sec\n"

		############ START of - Time in seconds for answering the question ##########
		answered_option="x"			# Initialize answered_option to some wrong answer
		####### Max 10 sec ##########
		time_left=10
		sec_before=$SECONDS
		read -t $time_left answered_option
		sec_after=$SECONDS
		time_taken=$(($sec_after-$sec_before))	
		############ END of - Time in seconds for answering the question ############

		right_option=`grep -w "$question_number_format)" answer_bank.txt | cut -d' ' -f2`


		if [ "$answered_option" == "$right_option" ]	# USE DOUBLE QUOTES for operands or DOUBLE BRACES if[[ ]], else sometimes error
		then
			echo -e "\t$question_number\t\t\t$answered_option\t\t\t$right_option\t\tCORRECT\t\t\t$time_taken" >> $user_answer_file
			let marks=$marks+1;
		else
			echo -e "\t$question_number\t\t\t$answered_option\t\t\t$right_option\t\tWRONG\t\t\t$time_taken" >> $user_answer_file

		fi

		############################  INCOMPLETE    ###############################

		echo -e "\n<< Press P for PREVIOUS \t\t\t\t\t Press N for NEXT >>"
		read next_previous

		if [ $next_previous == n -o $next_previous == N ]
		then
			# Nothing
			echo "Pressed N"

		elif [ $next_previous == p -o $next_previous == P ]
		then
			# Currently Nothing, but needs modification
			echo "Pressed P"
		fi
		############ This Module is just started. Need to elaborate ###############

		clear															# To clear screen

	done

	echo marks = $marks
	echo -e "\nmarks $marks" >> $user_answer_file
	##################################  END of - To pick ques from ques bank and print ques #############################

}
#########         END of -          function func_signin__take_test()          ##########



#########        START of - function func_signin__view_test()        ##########
function func_signin__view_test()
{
	signedin_user=$1
	if [ -e ${signedin_user}_answers.txt ]
	then
		echo "answer file exists\n"
		cat ${signedin_user}_answers.txt	

	else
		echo answer file doesnot exist
	fi

}
#########        END of - function func_signin__view_test()        ##########

#########        START of - option 1) function func_signin()        ##########
function func_signin()
{

	#echo function signin
	echo

	echo Enter Username
	read username_signin

	echo Enter Password
	stty -echo
	read password_signin 
	stty echo

	username_exist=`cut -d' ' -f1 userlog.txt | grep -w $username_signin`
	password_exist=`cut -d' ' -f2 userlog.txt | grep -w $password_signin`

#	echo grep status : $username_exist
	if [ -n "$username_exist" -a -n "$password_exist" ]
	then

		echo "Successfully Signed-in"
		echo -e "\nWelcome $username_signin\n"

		echo Select option
		echo Press 1 to Take Test
		echo Press 2 to View Test
		echo Press 3 to LogOut

		read login_option

		case $login_option in

			1)	echo Take Test
				func_signin__take_test $username_signin
				;;

			2) 	echo View Test
				func_signin__view_test $username_signin
				;;

			3)	echo Logging Out
				func_signin__log_out $username_signin
				;;

			*)  echo Unrecognized value		# While loop needed to go back and repeat
				echo Calling function Signin again
				func_signin
				;;

		esac

	else
		echo -e "\nUsername and Password donot match\n" 

	fi

	echo 


}
#########         END of - option 1) function func_signin()        ##########


#########        START of - option 2) function func_signup()        ##########
function func_signup()
{

	#echo function signup

	echo Enter Username
	read username_signup

	echo Enter Password
	stty -echo
	read password_signup
	stty echo

	echo $username_signup $password_signup >> userlog.txt

	echo -e "\nSuccessfully signed up\n"

}
#########       END  of - option 2) function func_signup()        ##########


########        START of - option 3) function func_exit()        ##########
function func_exit()
{

	#echo function exit

	exit 0
}
########        END of - option 3) function func_exit()        ##########

echo

########################## MAIN PROGRAM ###############################

flag_repeat=1

while [ $flag_repeat -eq 1 ]
do

	echo Press 1 to Signin
	echo Press 2 to Signup
	echo Press 3 to Exit

	read option_user_input


	case $option_user_input in

		1)	#echo calling signin function
			func_signin
			;;

		2)	#echo calling signup function
			func_signup
			;;

		3)	#echo calling exit function
			func_exit
			;;

		*)  echo Unrecognized value		# While loop needed to go back and repeat
			echo Program will exit
			func_exit
			;;
	esac

done

