package main

import (
	"github.com/gorilla/mux"
	"log"
	"net/http"
)

var database Users

func main() {
	log.Print("Init server...")

	database = readDatabase()

	router := mux.NewRouter()
	router.HandleFunc("/api", GetHomeEntryPoint ).Methods("GET")
	router.HandleFunc("/api/in", GettingInEndpoint).Methods("GET")
	router.HandleFunc("/api/out", GettingOutEndpoint).Methods("GET")
	router.HandleFunc("/api/door_open", DoorOpenedEndpoint).Methods("GET")
	router.HandleFunc("/api/user_enter", UserEnterEndpoint).Methods("PUT")
	router.HandleFunc("/api/advise_take_keys", AdviseTakeKeysEndpoint).Methods("GET")
	router.HandleFunc("/api/advise_leave_keys", AdviseLeaveKeysEndpoint).Methods("GET")
	log.Fatal(http.ListenAndServe(":3000", router))
}

