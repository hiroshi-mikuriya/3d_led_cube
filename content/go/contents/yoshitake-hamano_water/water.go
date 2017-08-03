package main

import (
	"encoding/csv"
	"github.com/tarm/goserial"
	"github.com/yoshitake-hamano/3d_led_cube_go"
	"log"
	"strconv"
	"time"
)

const LED_URL = "127.0.0.1:9001"
const WATER_COLOR = 255 << 0 // blue
const WATER_AVE = 16
const NUM_OF_RECORDS = 4

func main() {
	sc := &serial.Config{Name: "/dev/ttyACM0", Baud: 115200, ReadTimeout: time.Second}
	s, err := serial.OpenPort(sc)
	if err != nil {
		log.Fatal(err)
	}
	defer s.Close()

	reader := csv.NewReader(s)
	reader.Comma = '\t'

	log.Println("Connecting..." + LED_URL)
	led_cube_go.SetUrl(LED_URL)
	for {
		time.Sleep(60 * time.Millisecond)
		record, err := reader.Read()
		if err != nil {
			log.Print(err)
			continue
		}
		if len(record) != NUM_OF_RECORDS {
			log.Print("invalid length records: ", record)
			continue
		}
		xSlope, err := strconv.ParseFloat(record[0], 64)
		if err != nil {
			log.Print("xSlope: ", err)
			continue
		}
		zSlope, err := strconv.ParseFloat(record[2], 64)
		if err != nil {
			log.Print("zSlope: ", err)
			continue
		}
		log.Printf("%f %f", xSlope, zSlope)
		showWater(xSlope/100, zSlope/100)
	}
}

func showWater(xSlope, zSlope float64) {
	led_cube_go.Clear()
	for x := 0; x < led_cube_go.LED_WIDTH; x++ {
		for z := 0; z < led_cube_go.LED_DEPTH; z++ {
			xOrigin := led_cube_go.LED_WIDTH / 2
			zOrigin := led_cube_go.LED_DEPTH / 2
			vx := float64(x - xOrigin)
			vz := float64(z - zOrigin)
			inner := vx*xSlope + vz*zSlope
			yMin := int(WATER_AVE + inner)
			if yMin < 0 {
				yMin = 0
			}
			if led_cube_go.LED_HEIGHT < yMin {
				yMin = led_cube_go.LED_HEIGHT
			}
			for y := yMin; y < led_cube_go.LED_HEIGHT; y++ {
				led_cube_go.SetLed(x, y, z, WATER_COLOR)
			}
		}
	}
	led_cube_go.Show()
}
