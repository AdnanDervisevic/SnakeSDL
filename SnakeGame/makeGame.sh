#!/bin/bash

g++ -o Snake Player.cpp SpriteBatch.cpp SnakeGame.cpp SoundEffect.cpp Music.cpp BodyPart.cpp -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer