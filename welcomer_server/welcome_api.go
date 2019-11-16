package main

import (
	"fmt"
	"log"
	"net/http"
)

func GetHomeEntryPoint(w http.ResponseWriter, req *http.Request) {
	if _, err := fmt.Fprint(w, "API is online"); err != nil {
		log.Fatal("Error in API entry point.")
	}
}

func GettingInEndpoint(w http.ResponseWriter, req *http.Request) {
	if _, err := fmt.Fprint(w, "OK"); err != nil {
		log.Fatal("Error in Response")
	}
	log.Print("In")

}

func GettingOutEndpoint(w http.ResponseWriter, req *http.Request) {
	if _, err := fmt.Fprint(w, "OK"); err != nil {
		log.Fatal("Error in Response")
	}
	log.Print("Out")
	//run concurrently the tts so the response is returned earlier
	go func() {
		if isCold() {
			say("It's cold out there. Make sure to take a jacket!")
		} else {
			say("Enjoy your day!")
		}
	}()
}

func UserEnterEndpoint(w http.ResponseWriter, req *http.Request) {
	uid := req.Header["Uid"][0]
	database.userLogged(uid)
	log.Print("User ", database.getNameFromUid(uid), " entered")
}

func DoorOpenedEndpoint(w http.ResponseWriter, req *http.Request) {
	if _, err := fmt.Fprint(w, "OK"); err != nil {
		log.Fatal("Error in Response")
	}
	log.Print("door_open")
}
