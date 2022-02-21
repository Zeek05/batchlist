@echo off
color 0f
title Demonstration Program
if exist "demo" del "demo"
list "demo" /nl
list "demo" push "1"
list "demo" push "2"
list "demo" push "3"
list "demo" push "4"
list "demo" push "5"
list "demo" push "6"
list "demo" push "7"
list "demo" push "8"
list "demo" push "9"
list "demo" push "10"
list "demo" push "11"
list "demo" push "12"
list "demo" push "13"
list "demo" push "14"
list "demo" push "15"
list "demo" push "16"
list "demo" pop
list "demo" pop
list "demo" pop
list "demo" pop
list "demo" pop
list "demo" pop
list "demo" popback
list "demo" popback
list "demo" popback
list "demo" popback
list "demo" popback
list "demo" popback
echo.
echo Length of list:
list "demo" getlength
echo.
echo Contents of list:
list "demo" print
echo.
pause
