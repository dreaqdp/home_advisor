package main

import (
	"github.com/gorilla/mux"
	"log"
	"net/http"
)

func main() {
	log.Print("Init server...")

	router := mux.NewRouter()
	router.HandleFunc("/api",				GetHomeEntryPoint ).Methods("GET")
	router.HandleFunc("/api/in",	DoorOpenedEndpoint).Methods("GET")
	router.HandleFunc("/api/out",	DoorOpenedEndpoint).Methods("GET")
	router.HandleFunc("/api/door_open",	DoorOpenedEndpoint).Methods("GET")
	log.Fatal(http.ListenAndServe(":3000", router))
}

