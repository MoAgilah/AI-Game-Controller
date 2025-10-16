# 🎮 AI Game Controller
### Neuroevolved Agent for a 2D Platformer (C++ / SFML)

| **AI-Controlled Mode** | **Manual Mode** |
|---|---|
| ![Manual Demo](https://raw.githubusercontent.com/MoAgilah/AI-Game-Controller/master/SFMLGameEngine/Previews/GIFs/ManualDemo.gif) | ![AI Demo](https://raw.githubusercontent.com/MoAgilah/AI-Game-Controller/master/SFMLGameEngine/Previews/GIFs/ManualDemo.gif) |



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
You can download the latest standalone executabke here:

👉 [Download Standalone Executable](https://raw.githubusercontent.com/MoAgilah/AI-Game-Controller/master/SFMLGameEngine/Bin/AI-Game-Controller.zip)


After downloading:

- Extract the .zip file.
- Open the folder and run the executable:
  - **Windows:** `AI_Game_Controller.exe`
- Use the instructions below to play through debug mode or toggle AI simulation mode.

---

## 🕹️ How to Start / Mode Select
- Launch the game.
- When prompted, use **Up / Down** to choose:
  - **Play Manually (Debug Mode)**
  - **Start Simulation (AI Mode)**
- Press **Enter** to confirm your selection.

---

## 🌍 Global Actions

| Action            | Key     |
|--------------------|---------|
| Open / Close Pause Menu | **Space** |
| Exit Game (any time)    | **Esc**   |

---

## ⏸️ Pause Menu
A global **Pause Menu** is available in both modes and functions just like the main menu.  
Use **Up / Down** to navigate and **Enter** to select.

**Options:**
- **Resume** – return to the game.  
- **To Title** – return to the main menu.  
- **Quit** – close the game entirely.  

---

## 🎮 Manual Mode Controls

| Action                     | Key |
|-----------------------------|-----|
| Move Left                   | ←   |
| Move Right                  | →   |
| Look Up                     | ↑   |
| Crouch                      | ↓   |
| Run (hold to keep speed)    | **S** |
| Jump                        | **A** |
| Spin Jump (SJUMP)           | **D** |

---

## 🤖 AI Mode
- In **AI Mode**, the controller automatically plays the game using neural-network-based behavior.  
- Player input is disabled, but you can still global actions as required.

---

## 🧩 How It Works
1. The program creates a **population of neural networks**—each representing an AI controller.  
2. Every controller plays the level and is **scored by fitness** (distance, survival time, completion).  
3. The best controllers are **selected, mutated, and crossed over** to form the next generation.  
4. Over time, the AI learns timing, jumping, and obstacle avoidance.  
5. The evolved agent is tested against human gameplay for comparison.

---

## 🧱 Folder Structure
```
AI-Game-Controller\
|
SFMLGameEngine
+-- Bin/          # Downloadable builds (executables)
+-- Code/         # Source code (C++)
+-- Resources/    # Fonts, Sprites, Shaders, Outputs, etc.
+-- Previews/     # Screenshots, GIFs
\-- README.md     # Project documentation
```

---

## 🧠 Learning Objectives
- Design and implement a **2D platformer** using SFML and modern C++ conventions.  
- Apply **neuroevolution (genetic algorithm + ANN)** to gameplay control problems.  
- Evaluate and compare **AI and human playstyles** using performance metrics.  
- Refactor legacy academic code into a **maintainable, modular** structure.  
- Log, visualize, and interpret AI learning results.  

---

## 🧩 System Requirements
| Platform | Minimum Spec |
|-----------|---------------|
| **Windows** | Windows 10+, SFML runtime included |
| **CPU** | Dual-core 2.0GHz |
| **RAM** | 4GB |
| **GPU** | Any with OpenGL 2.1 support |

---

## 🧱 Game States

| State | Description | Preview |
|--------|-------------|----------|
| **TitleState** | Displays **“Press Any Key To Start”** and transitions to the Main Menu upon key press.|
| **MainMenuState** | Title and mode selection (**Manual** or **AI**).|
| **LoadingState** | Preloads textures, world objects, and assets before the game begins.|
| **MainState** | Active gameplay, player and AI logic, and camera management.|
| **PauseMenuState** | Opens with **Space**, offering Resume, Title, or Quit options.|

---

## 📚 Future Enhancements
- Add multiple **level files** and adjustable parameters.  
- Implement **save/load** for trained neural networks.  
- Display **visual AI debugging overlays**.  
- Package **cross-platform release builds**.

---

## 👤 Author
**Mohamed Agilah**  
🎓 Games Programmer & AI Developer  
🌐 [Portfolio Website](https://moagilah.com/)  
📧 Contact: agilahmohamed@gmail.com  

> *Project archived for educational and portfolio purposes (October 2025).*
