# Customers Debts Management System

This program is a simple customer management system that allows you to read customer data from a CSV file, display it in a list, and perform set and select operations on the data.

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
        field can be any of the rows in the table (first and second name, id , phone , date, debt ).
        operator can be only one of this: <, >, !=, = .
        no spaces allowed before and after the operator.

    -   `set`
        write a set query in this format:
        `set first name=<value>,second name=<value>,id=<value>,phone=<value>,date=<value>,debt=<value>`
        you need to provide all the fields,
        with the correct value, and in the right format.
        no spaces are allowed before and after the operator and commas.