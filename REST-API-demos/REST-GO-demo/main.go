package main

import (
	"./services"
	"fmt"
)

func main() {
	fmt.Println("火币")

	fmt.Println(services.GetTradeDetail("btcusdt"))
}
