package main

import (
	"log"
)

type User struct {
	uuid string
	pathNn string
	name string
	in bool
}
type Users struct {
	users []User
}

func readDatabase() Users {
	u := Users{
		make([]User, 2),
	}
	u.users[0] = User{
		"41170242148",
		"",
		"Andrea",
		false,
	}

	u.users[1] = User{
		"8919549219",
		"",
		"Guillem",
		false,
	}
	return u;
}
func (u User) logUser(){
	log.Print("User: ")
	log.Print("	Name: ", u.name)
	log.Print("	uuid: ", u.uuid)
	log.Print("	pathNn: ", u.pathNn)
}

func (db Users) logUsers(){
	for _, u := range db.users{
		u.logUser()
	}
}

func (db Users) userLogged(uid string){
	for _, u := range db.users {
		if u.uuid == uid {
			u.in = !u.in
			return
		}
	}
}

func (db Users) getNameFromUid(uid string) string {
	for _, u := range db.users {
		if u.uuid == uid {
			return u.name
		}
	}
	return "Not found"
}

