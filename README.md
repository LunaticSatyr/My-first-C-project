TLDR: admin name: "a", password: "b"; student name and password at lines 2372-2405 in the source code.

# My first C++ project
A student enrolment and management system

I was introduced to C++ by a little brother, and started this project (his assignment) 5 weeks into studying C++.
Despite me taking 3 weeks to do it, there are a lot of basic shortcomings and flaws in the code (which I'll list down below).
Nonetheless, it was a fun experience and I've learned a lot throughout the process.

I've learned and implemented:
- a basic login system to allow CRUD operations by users on the student list and admin list,
- RegEx to validate user input,
- a dynamic layout that resizes to string of different length for student list / examination records,
- an auto-incrementing student ID (using map) and self-updating student trimester (using localtime_r), and
- CSV file handling to retain and reuse user data from previous sessions.

My shortcomings and flaws which I only realized 1 or 2 days before the deadline:
- C++ has more data types such as "size_t" which is more appropriate to be used than "int" in many sections of this project.
- Linked list is better than vector to store students' game history
- Could have linked my project to a SQL database

Without a doubt, there must have been more shortcomings that I do not realize or do not know how to put into words.
For anyone interested, I am honored if you try the program out without looking at the messy code :P
Stay safe, and stay entertained! ^_^
