#! /usr/bin/env atf-sh

atf_test_case create_user_tst1

create_user_tst1_head()
{

	atf_set "descr" "create_user_tst1"

}


create_user_tst1_body()
{

	useradd tst1

	if [ $? = "0" ]; then

		atf_pass

	else

		atf_fail "useradd tst1 failed"


	fi

}


atf_init_test_cases()
{


	atf_add_test_case create_user_tst1

}
