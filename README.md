# 🎮 AI Game Controller
### Neuroevolved Agent for a 2D Platformer (C++ / SFML)

![Gameplay Preview](docs/ai_gameplay.gif)

---

## 🧠 Overview
The **AI Game Controller** project is a refactored version of my Master’s dissertation that explores how an **AI agent** can learn to play a 2D platformer level in a way that resembles human gameplay.  

The system uses a **neuroevolutionary algorithm** — evolving artificial neural networks through genetic selection — to teach an AI controller how to complete a *Super Mario–style* level built with **SFML** in C++.  
The final challenge asks: *Can observers tell if the player is human or AI?*

---

## 🚀 Features
- 🧩 **AI Controller:** Learns movement and jump patterns using a genetic algorithm (NEAT-inspired).  
- 🎮 **Human Play Mode:** Manual controls for testing and comparison.  
- 🧱 **Custom 2D Engine:** Core loop, input mapping, physics, and collision detection built from scratch.  
- 📊 **Performance Logging:** Option to output generation and fitness data for analysis.  
- 🧾 **Archived Build:** Ready-to-play executable available for download.  

---

## 📦 Download & Run
You can download the latest build here:

👉 **[Download Game Build (ZIP)](https://github.com/MoAgilah/AI-Game-Controller/releases/latest)**

After downloading:
1. Extract the `.zip` file.  
2. Open the folder and run the executable:  
   - **Windows:** `AI_Game_Controller.exe`  
   - **Linux:** `./AI_Game_Controller` (make executable if needed: `chmod +x AI_Game_Controller`)  
3. Use the controls below to play or toggle AI mode.  

---

## 🕹️ Controls
| Action | Key |
|--------|-----|
| Move Left | ← |
| Move Right | → |
| Jump | Space |
| Toggle AI / Player | A |
| Restart Level | R |
| Exit Game | Esc |

---

## 🧩 How It Works
1. The program creates a **population of neural networks**—each representing an AI controller.  
2. Every controller plays the level and is **scored by fitness** (distance, survival time, completion).  
3. The best controllers are **selected, mutated, and crossed over** to form the next generation.  
4. Over time, the AI learns timing, jumping, and obstacle avoidance.  
5. The evolved agent is tested against human gameplay for comparison.

---

## 🧱 Folder Structure
