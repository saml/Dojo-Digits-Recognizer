package main

import (
	"encoding/csv"
	"fmt"
	"io"
	"log"
	"os"
	"strconv"
	"time"
)

func readData(filename string) [][]int {
	var rows [][]int

	file, err := os.Open(filename)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	c := csv.NewReader(file)
	c.Read()

	for {
		data := []int{}

		row, err := c.Read()
		if err == io.EOF {
			break
		} else if err != nil {
			log.Fatal(err)
		}

		for i := 0; i < len(row); i++ {
			v, _ := strconv.Atoi(row[i])
			data = append(data, v)
		}

		rows = append(rows, data)
	}
	return rows
}

func match(validation [][]int, training [][]int) int {
	matches := 0
	for _, xs := range validation {
		minSum := 0
		minLabel := 0
		for _, ys := range training {
			d := 0
			xr := xs[1:]
			yr := ys[1:]
			for i := 0; i < len(xr); i++ {
				t := xr[i] - yr[i]
				d += t * t
			}
			if minSum == 0 || d < minSum {
				minSum = d
				minLabel = ys[:1][0]
			}
		}
		if minLabel == xs[:1][0] {
			matches++
		}
	}
	return matches
}

func main() {
	valid := readData("validationsample.csv")
	train := readData("trainingsample.csv")

	start := time.Now()
	matches := match(valid, train)

	fmt.Printf("Match: %v%% \n", float64(matches*100)/float64(len(valid)))
	fmt.Printf("Took: %s \n", time.Since(start).String())
}
