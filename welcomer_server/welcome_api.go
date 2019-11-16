package main

import (
	"fmt"
	"log"
	"net/http"
)

func GetHomeEntryPoint (w http.ResponseWriter, req *http.Request) {
	if _, err := fmt.Fprint(w, "API is online"); err != nil {
		log.Fatal("Error in API entry point.")
	}
}

func GettingInEndpoint (w http.ResponseWriter, req *http.Request) {
	if _, err := fmt.Fprint(w, "OK"); err != nil {
		log.Fatal("Error in Response")
	}


}

func GettingOutEndpoint (w http.ResponseWriter, req *http.Request) {
	if _, err := fmt.Fprint(w, "OK"); err != nil {
		log.Fatal("Error in Response")
	}

}

func DoorOpenedEndpoint (w http.ResponseWriter, req *http.Request) {
	if _, err := fmt.Fprint(w, "OK"); err != nil {
		log.Fatal("Error in Response")
	}

}