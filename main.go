/******************************************************************************/
/*
* @file   main.go
* @author Aditya Harsh
* @email  mrsaturnsan@gmail.com
* @brief  Given a set of numbers, the program returns the sum of their squares.
*/
/******************************************************************************/

package main

import (
    "bufio"
    "fmt"
    "os"
    "log"
    "strconv"
    "strings"
)

/**
 * @brief Error function
 * 
*/
func check(e error) {
    if e != nil {
        panic(e)
    }
}

/**
 * @brief Recursive function that sums up the int values 
 *        within the line[]
 * 
*/
func sum(line *[]string, times int, total *int) {
    if times == 0 {
        return
    }

    // convert the string into an int
    val, err := strconv.Atoi((*line)[times - 1])
    check(err)

    // make sure range is valid
    if val < -100 || val > 100 {
        panic("Invalid parameter!")
    }

    // make sure not to square negative values
    if val > 0 {
        *total += val * val
    }

    // repeat function
    sum(line, times - 1, total)
}

/**
 * @brief Responsible for setting up string, calculating, 
 *        and printing output
 * 
*/
func calculate(line *string, times int) {
    // split up the string
    s := strings.Split((*line), " ");

    // stores the sum of squares
    var total = 0

    // sum up the strings
    sum(&s, times, &total)

    // print the output
    fmt.Println(total)
}

/**
 * @brief Calculates sum of the given number's squares
 * 
*/
func readLine(scanner *bufio.Scanner, times int) {
    if times == 0 {
        return
    }

    // read amount of numbers
    if scanner.Scan() {
        line := scanner.Text()

        if line == "" {
            panic("Invalid data!")
        }

        numbers, err := strconv.Atoi(line)
        check(err)

        if (numbers > 0 && numbers <= 100) {
            // read the next line
            if scanner.Scan() {
                line = scanner.Text()

                if line == "" {
                    panic("Invalid data!")
                }

                // calculate the sum of squares
                calculate(&line, numbers)
            } else {
                // error occured, exit out
                return
            }
        }
    } else {
        // error occured, exit out
        return;
    }

    // read the next line
    readLine(scanner, times - 1)
}

/**
 * @brief Calculates sum of the given number's squares
 * 
*/
func main() {
    // create the scanner
    scanner := bufio.NewScanner(os.Stdin)

    // get the number of operations
    if scanner.Scan() {
        line := scanner.Text()
        if line == "" {
            panic("Invalid data!")
        }

        times, err := strconv.Atoi(line)
        check(err)

        // make sure valid
        if times > 0 && times <= 100 {
            readLine(scanner, times)
        }
    }

    // check for errors
    if err := scanner.Err(); err != nil {
        log.Println(err)
    }
}
