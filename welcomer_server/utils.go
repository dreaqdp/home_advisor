package main

import "github.com/hegedustibor/htgo-tts"

func isCold() bool {
	return true
}

func say(toSay string) error {
	speech := htgotts.Speech{Folder: "audio", Language: "en"}
	return speech.Speak(toSay)
}
