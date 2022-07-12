#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <vector>
#include <map>

#include "DummyRecursionSolver.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Background.hpp"

using std::string;
using std::vector;
using std::map;

namespace graphics
{
	class Scene
	{
		public:
		/**
		 * Creates the scene.
		 * 
		 * @param gameObjects List of all the game objects to be included in the scene.
		 * @param camera Initial camera for the scene.
		 * @param availableCameras All the available cameras for the scene
		 * @param background Background of the scene.
		 * @param name Unique name of the scene.
		 * @param id Unique id of the scene.
		 */
		Scene(vector<GameObject> gameObjects, int camera, vector<Camera> availableCameras, Background background, string name, int id);
		/**
		 * Gets a game object by its name.
		 * 
		 * @param name Name of the game object to retrieve.
		 * @return The GameObject.
		 */
		GameObject getGameObjectByName(string name);
		/**
		 * Gets the scene camera.
		 * 
		 * @return Camera of the scene.
		 */
		Camera getCamera();
		/**
		 * Gets all the available cameras in the scene.
		 * 
		 * @return All the available cameras for the scene.
		 */
		vector<Camera> getAvailableCameras();
		/**
		 * Gets the background of the scene.
		 * 
		 * @return Background of the scene.
		 */
		Background getBackground();
		/**
		 * Gets the name of the scene.
		 * 
		 * @return Name of the scene.
		 */
		string getName();
		/**
		 * Updates the game object by using its name as indexer.
		 * 
		 * @param gameObject The new GameObject that will replace the old one.
		 * @param name The name of the GameObject.
		 */
		void setGameObjectByName(GameObject gameObject, string name);
		/**
		 * Set the camera for the scene.
		 * 
		 * @param cameraPos The position of the available camera to be used.
		 */
		void setCamera(int cameraPos);
		/**
		 * Modify the available camera at the specified index.
		 * 
		 * @param camera The new camera that will substitute the old one.
		 * @param pos The position of the camera to modify.
		 */
		void modifyCamera(Camera camera, unsigned int pos);
		/**
		 * Set the background for the scene.
		 * 
		 * @param background The background to be set.
		 */
		void setBackground(Background background);

		private:
		/**
		 * Vector of the names of the game objects.
		 */
		vector<string> gameObjectsNames;
		/**
		 * Map between names and game objects.
		 */
		map<string, GameObject> mapNamesGameObjects;
		/**
		 * Vector of all the game objects in the scene.
		 */
		vector<GameObject> gameObjects;
		/**
		 * The camera defining the view.
		 */
		Camera camera;
		/**
		 * The list of all the camera that can be used.
		 */
		vector<Camera> availableCameras;
		/**
		 * The background for the scene.
		 */
		Background background;
		/**
		 * The name of the scene.
		 */
		string name;
		/**
		 * The id of the scene.
		 */
		int id;
	};
}

#endif