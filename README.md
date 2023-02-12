# Customers Debts Management System

This is a customer-management-system program that allows you to read customers data from a CSV file, display it in a list, and perform set and select operations on the data.

# How to run

To run the program:

1. make local
2. ./local <file_name.txt>


# Operations

Once the program is running, you can perform the following operations:

- `select` - search in the list with select query
- `set`    - update customer debt or creating a new one
- `print`  - print the list of customers debts orderd by their size
- `quit`   - exit the program


# How to use

    -   `select`
        write a select query in this format:
        `select <field><operator><value>`.
        for example: select debt<100.
        field can be any of the rows in the table 
        (first and second name, id , phone , date, debt ).
        operator can be only one of this: <, >, !=, = .
        no spaces allowed before and after the operator.

    -   `set`
        write a set query in this format:
        `set first name=<value>,second name=<value>,id=<value>,
        phone=<value>,date=<value>,debt=<value>`
        you need to provide all the fields,
        with the correct value, and in the right format.
        no spaces are allowed before and after the operator and commas.

# Project Flow & Modules brief

The system base works on string processing. Receiving the string can be done in 2 options; Each branch contains its own main file that is compiled with the central module system. Branch master contains the local main that receives the string from the user using the prompt, and branch part_2 that contains the server that receives the string from the client's prompt (which is another file run in the second branch) using sockets.

### Moduls:

    `local` \ `client - server` -> incharge of calling the functions for user interaction, csv proccessing, and sending the useer strings of queries to be proccessed.

    
    `csv`   ->  incharge of proccessing the file once the local or server are booting up.
                correct an valid lines are sending to the list module.

    `list`  ->  incharge of building and adding the list, printing, and maintaining it by debt order.

    `userinput` ->  incharge of the proccessing the user input strings, breaking the string and analyse it. if nedded it uses the list module to update the list.

    `utils` -> holdes all the validations and callback functions, which are called from across the program.


         +------------+              +-------+           +--------+           
         |            |              |       |           |        |
    +----|   local    |----------->  |  csv  |-------->  |  list  |  
    |    |            |    +------>  |       |-------+   |        |
    |    +------------+    |         +-------+       |   +--------+
    |                      |                         |       ^
    |    +------------+    |                         |       |
    |    |            |----+                         |       |
    |    |   server   |              +------------+  |       |      +---------+ 
    |    |            |--------->    |            |  +-------+--->  |         |
    |    +------------+    +---->    | userinput  |----------+--->  |  utils  |  
    |                      |         |            |                 |         |
    +----------------------+         +------------+                 +---------+