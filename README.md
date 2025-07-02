# Photon Integration Sample

**Photon** is a networking engine and multiplayer platform that handles all communication on its own servers. In a typical multiplayer setup, you implement the core application (gameplay) logic within your UNIGINE project, while **Photon** manages the networking layer.

This sample includes two **Photon** products:

- **Photon Realtime** - real-time communication between multiple players over the network. Suitable for multiplayer shooters, racing games, and other latency-sensitive applications.
- **Photon Chat** – public/private in-game chat system.

The sample includes the following functional elements:

- **Authorization Window** – Simple login interface where users enter a nickname; extendable to include more advanced authentication (e.g., passwords).
- **Lobby Window** – A UI to create rooms and browse the list of available rooms to join.
- **Gameplay World** – The main interactive scene where objects can be manipulated in real time + chat panel.

This gives a solid foundation for for building your own UNIGINE-based multiplayer applications using **Photon**'s networking services. From there, you can explore and integrate additional **Photon** products as needed.

## Data Transfer via Photon

**Photon** doesn’t natively support UNIGINE types. To send such data over the network, it must be serialized before transmission and deserialized upon receipt. This sample provides utility code for that, including:

1. **Common data types** supported by Photon - listed [here](https://doc-api.photonengine.com/en/cpp/current/) under *Common-cpp*.
2. **More complex data types** (e.g. transformation matrices) requiring explicit registration for custom serialization.

## How to Run the Sample

### Prerequisites

- [**UNIGINE SDK Browser**](https://developer.unigine.com/en/docs/latest/start/installing_sdk?rlang=cpp) (latest version)
- **UNIGINE SDK Community** or **Engineering** edition (**Sim** upgrade supported)
- **Visual Studio 2022** (recommended)
  
### Step-by-Step Guide

To get started with the **Photon C++ Sample**:

1. **Clone or download** the sample.

2. **Open SDK Browser** and make sure you have the latest version.

3. **Add the sample project to SDK Browser**:
   - Go to the *My Projects* tab.
   - Click *Add Existing*, select the `.project` file from the cloned folder (matching your OS - `*-win-*`/`*-lin-*`, edition, precision), and click *Import Project*.

     ![Add Project](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/add_project.png)

> [!NOTE]
> If you're using **UNIGINE SDK *Sim***, select the ***Engineering*** `*-eng-sim-*.project` file when importing the sample. After import, you can upgrade the project to the **Sim** version directly in SDK Browser - just click *Upgrade*, choose the SDK **Sim** version, and adjust any additional settings you want to use in the configuration window that opens.

4. **Repair the project**:
   - After importing, you'll see a **Repair** warning - this is expected, as only essential files are stored in the Git repository. SDK Browser will restore the rest.

   ![Repair Project](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/photon_repair.png)
   
    - Click *Repair* and then *Configure Project*.

6. **Download Photon Realtime C++ Windows SDK** (**v5.0.13 Build 2**) from [photonengine.com](https://www.photonengine.com/). If you don’t have an account, you’ll need to register first.

  ![Photon SDKs](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/sdks.jpg)
 
6. **Copy Photon SDK files**:
   - From downloaded SDK’s `/source`, copy the following folders into your project’s `source/PhotonSDK`:
     -   `Chat-cpp`
     -   `Common-cpp`
     -   `LoadBalancing-cpp`
     -   `Photon-cpp`
     -   `PhotonVoice-cpp`.
>[!TIP]
>You can quickly access your project via SDK Browser by clicking the three dots next to your project's name and selecting _Open Folder_.
>
>![Project Folder](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/project_folder.png)

7. **Create App IDs in Photon Dashboard**:
   - Go to the Dashboard at [photonengine.com](https://www.photonengine.com/) (the tab in the top right corner) and click **CREATE A NEW APP**.
     
   ![Photon App](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/photon_app.png)
   
   - Create two apps: ***Realtime Photon SDK*** and ***Chat Photon SDK***.

   | ![Realtime App](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/realtime_app.png)| ![Chat App](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/chat_app.png)|
   |---|---|
   | <sup>Creating a Realtime Photon App</sup> | <sup>Creating a Chat Photon App</sup>|
   
   - Copy the generated App IDs in your sample project. The same ID is used for every instance (i.e. other participants don't need to create their own apps).
     
   ![Photon ID](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/photon_app_id.png)
   
8. **Add App IDs to the sample project**: open `data/application_params.json` in your project `/data` folder and paste IDs:

   ```json
   {
     "realtime_application_id": "YOUR_REALTIME_ID",
     "realtime_application_version": "1.0",
     "chat_application_id": "YOUR_CHAT_ID",
     "chat_application_version": "1.0"
   }

9. **Open** the project in your IDE:

    - Launch the recommended Visual Studio 2022 (other C++ IDE with CMake support can be used as well).
    - Load the folder containing `CMakeLists.txt`. If everything is set up correctly, the `CMakeLists.txt` file will be highlighted in **bold** in the *Solution Explorer* window, indicating that the project is ready to build.

> [!WARNING]
   > By default, the project uses **single precision (float)**. To enable **double precision**, modify `CMakeLists.txt`:
   >```diff
   > - set(UNIGINE_DOUBLE False CACHE BOOL "Double coords")
   > + set(UNIGINE_DOUBLE True CACHE BOOL "Double coords")
   >```
   > Make sure this setting matches the `.project` file you selected (e.g., `*-double.project` for double precision builds).

10. **Build** and **Run** the project.

### If the sample fails to run:
- Double-check all setup steps above to ensure nothing was skipped.
- Check that `UNIGINE_DOUBLE` in `CMakeLists.txt` matches the current build type (double/float).
- Use the correct `.project` file for your SDK edition/platform.
- Verify your SDK version is not older than the project's specified version.
- If CMake issues occur in Visual Studio, right-click the project and select: **Delete Cache and Reconfigure** and then **Build** again.

## How to Use the Sample

1. Run the sample and enter a nickname in the authorization form → click *Join Lobby*.

![Photon Authorization](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/auth_app.png)  

2. If no rooms exist, create one (*Create Room*).

![Photon Create Lobby](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/lobby_empty.png)

3. Other users with the same App IDs in `data/application_params.json` will see and can join it.

![Photon Lobby](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/lobby_room.png)

4. Double‑click the room name to enter.

5. In the world, control the material ball with:
   - **WASD** to move
   - **QE** to rotate
   - Left click to shoot.
     
6. Use the chat panel:
   - Send global messages
   - Send private messages by typing `@username`.

![Photon Messages](https://developer.unigine.com/en/docs/latest/sdk/api_samples/third_party/photon/app_world.jpg)

7. As the health bar is empty, click *Leave* to return to *Lobby*.