#!/bin/bash
gcc nprep_norev.c -o r4_test

#ここからModel1
echo --Model1/dat0-- > result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t10 ="$(time (./r4_test "Model1/dat0/idata" "Model1/dat0/odata" ) 2>&1 1>/dev/null )"
echo $t10
echo 

echo "" >> result.txt
echo --Model1/dat1-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t11 ="$(time (./r4_test "Model1/dat1/idata" "Model1/dat1/odata" ) 2>&1 1>/dev/null )"
echo $t11

echo "" >> result.txt
echo --Model1/dat2-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t12 ="$(time (./r4_test "Model1/dat2/idata" "Model1/dat2/odata" ) 2>&1 1>/dev/null )"
echo $t12

echo "" >> result.txt
echo --Model1/dat3-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t13 ="$(time (./r4_test "Model1/dat3/idata" "Model1/dat3/odata" ) 2>&1 1>/dev/null )"
echo $t13

echo "" >> result.txt
echo --Model1/dat4-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t14 ="$(time (./r4_test "Model1/dat4/idata" "Model1/dat4/odata" ) 2>&1 1>/dev/null )"
echo $t14

echo "" >> result.txt
echo --Model1/dat5-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t15 ="$(time (./r4_test "Model1/dat5/idata" "Model1/dat5/odata" ) 2>&1 1>/dev/null )"
echo $t15

echo "" >> result.txt
echo --Model1/dat6-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t16 ="$(time (./r4_test "Model1/dat6/idata" "Model1/dat6/odata" ) 2>&1 1>/dev/null )"
echo $t16

echo "" >> result.txt
echo --Model1/dat7-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t17 ="$(time (./r4_test "Model1/dat7/idata" "Model1/dat7/odata" ) 2>&1 1>/dev/null )"
echo $t17

echo "" >> result.txt
echo --Model1/dat8-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t18 ="$(time (./r4_test "Model1/dat8/idata" "Model1/dat8/odata" ) 2>&1 1>/dev/null )"
echo $t18

echo "" >> result.txt
echo --Model1/dat9-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t19 ="$(time (./r4_test "Model1/dat9/idata" "Model1/dat9/odata" ) 2>&1 1>/dev/null )"
echo $t19

#ここからModel2
echo "" >> result.txt
echo --Model2/dat0-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t20 ="$(time (./r4_test "Model2/dat0/idata" "Model2/dat0/odata" ) 2>&1 1>/dev/null )"
echo $t20

echo "" >> result.txt
echo --Model2/dat1-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t21 ="$(time (./r4_test "Model2/dat1/idata" "Model2/dat1/odata" ) 2>&1 1>/dev/null )"
echo $t21

echo "" >> result.txt
echo --Model2/dat2-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t22 ="$(time (./r4_test "Model2/dat2/idata" "Model2/dat2/odata" ) 2>&1 1>/dev/null )"
echo $t22

echo "" >> result.txt
echo --Model2/dat3-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t23 ="$(time (./r4_test "Model2/dat3/idata" "Model2/dat3/odata" ) 2>&1 1>/dev/null )"
echo $t23

echo "" >> result.txt
echo --Model2/dat4-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t24 ="$(time (./r4_test "Model2/dat4/idata" "Model2/dat4/odata" ) 2>&1 1>/dev/null )"
echo $t24

echo "" >> result.txt
echo --Model2/dat5-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t25 ="$(time (./r4_test "Model2/dat5/idata" "Model2/dat5/odata" ) 2>&1 1>/dev/null )"
echo $t25

echo "" >> result.txt
echo --Model2/dat6-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t26 ="$(time (./r4_test "Model2/dat6/idata" "Model2/dat6/odata" ) 2>&1 1>/dev/null )"
echo $t26

echo "" >> result.txt
echo --Model2/dat7-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t27 ="$(time (./r4_test "Model2/dat7/idata" "Model2/dat7/odata" ) 2>&1 1>/dev/null )"
echo $t27

echo "" >> result.txt
echo --Model2/dat8-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t28 ="$(time (./r4_test "Model2/dat8/idata" "Model2/dat8/odata" ) 2>&1 1>/dev/null )"
echo $t28

echo "" >> result.txt
echo --Model2/dat9-- >> result.txt
echo "" >> result.txt
echo "pos,err" >> result.txt
t29 ="$(time (./r4_test "Model2/dat9/idata" "Model2/dat9/odata" ) 2>&1 1>/dev/null )"
echo $t29