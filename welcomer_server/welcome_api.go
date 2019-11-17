package main

import (
	"fmt"
	"github.com/PuerkitoBio/goquery"
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
	log.Print("Out")

	res, err := http.Get("http://192.168.1.118/")
	if err != nil {
		log.Print("Error sending predict request. Error was ", err.Error())
		return
	}

	defer res.Body.Close()
	if res.StatusCode != 200 {
		log.Fatalf("status code error: %d %s", res.StatusCode, res.Status)
	}

	// Load the HTML document
	doc, err := goquery.NewDocumentFromReader(res.Body)
	if err != nil {
		log.Fatal(err)
	}
	s, err := doc.Html()
	if err != nil {
		log.Print("cannot get html from resp")
		return
	}
	for _, c := range  s {
		if c == '1'{
			say("Have a great day!")
			return
		}else if c == '2'{
			say("It's cold outside. Remember to take a jacket!")
			return
		}
	}
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

func AdviseLeaveKeysEndpoint(w http.ResponseWriter, req *http.Request) {
	log.Print("keys forgotten to leave")
	say("You forgot to leave your keys!")
}

func AdviseTakeKeysEndpoint(w http.ResponseWriter, req *http.Request){
	log.Print("keys forgotten to take")
	say("You forgot to take your keys!")
}


