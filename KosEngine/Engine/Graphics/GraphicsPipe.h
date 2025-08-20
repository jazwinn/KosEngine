/******************************************************************/
/*!
\file      GraphicsPipe.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      31st Jan, 2025
\brief     The GraphicsPipe class provides a high-level interface for managing and executing OpenGL rendering,
           including shaders, framebuffers, VAOs, and drawing operations. It supports various graphical
           data types, such as meshes, textures, debug drawing, and text rendering, and includes functionality
           for animation and frame management.

\details   This class is designed to handle multiple rendering operations, including instanced drawing,
           managing multiple textures, and setting up framebuffers for offscreen rendering. It also includes
           functions for managing shader programs and rendering text using FreeType. Debug and text data structures
           are implemented to organize different rendering contexts.

           Dependencies:
           - GLEW for OpenGL extension loading.
           - GLFW for window and context management.
           - GLM for matrix operations and transformations.
           - AssetManager for loading assets, such as fonts and images.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef GRAPHPIPE_H
#define GRAPHPIPE_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "../Config/pch.h"
#include "../Asset Manager/AssetManager.h"
#include "../Helper/Helper.h"

namespace graphicpipe {

    struct EmitterData
    {
        int m_noOfParticles{};      ///< Number of particles emitted.
        float m_lifeSpan{};        ///< Lifespan of each particle in seconds.
        glm::vec2 m_position{};    ///< Emitter's position in world space.
        glm::vec2 m_velocity{};    ///< Initial velocity of emitted particles.
        glm::vec2 m_acceleration{}; ///< Acceleration applied to particles over time.
        glm::vec2 m_scale{};       ///< Scale of emitted particles.
        glm::vec4 m_color{};       ///< Initial color of emitted particles.
        float m_rotation{};        ///< Initial rotation of emitted particles in degrees.
        float m_coneRotation{};    ///< Rotation of the emission cone in degrees.
        float m_coneAngle{};       ///< Spread angle of emitted particles in degrees.
        float m_randomFactor{};    ///< Randomness applied to particle attributes.
        unsigned int m_textureID{}; ///< Texture ID for particle rendering.
        int m_stripCount{};        ///< Number of sprite strip frames.
        int m_frameNumber{};       ///< Current animation frame.
        int m_framesPerSecond{};   ///< Animation speed in frames per second.
        bool m_loopAnimation{};    ///< Whether the animation should loop.
        int m_layer{};             ///< Rendering layer order.
        float m_friction{};        ///< Friction applied to slow down particles.
    };

    struct ParticleData
    {
        float m_lifeSpan;            ///< Remaining lifespan of the particle.
        float m_rotation;            ///< Current rotation of the particle in degrees.
        float m_isActive;            ///< Flag indicating whether the particle is active.
        int m_textureID;             ///< Texture ID used for rendering the particle.
        int m_stripCount;            ///< Number of frames in the sprite strip.
        int m_frameNumber;           ///< Current frame of animation.
        int m_layer;                 ///< Rendering layer of the particle.
        float m_initialEmissionAngle; ///< Initial angle at which the particle was emitted.
        glm::vec2 m_position;        ///< Current position of the particle.
        glm::vec2 m_velocity;        ///< Current velocity of the particle.
        glm::vec2 m_acceleration;    ///< Acceleration applied to the particle.
        glm::vec2 m_scale;           ///< Scale of the particle.
        glm::vec4 m_color;           ///< Color of the particle.
        float m_friction;            ///< Friction applied to slow the particle down.
        int m_framesPerSecond{};     ///< Animation speed in frames per second.
        float m_animationTimer{};    ///< Timer for animation updates.
        float padding3{};            ///< Padding to align memory layout.
    };

    struct LightingData
    {
        glm::mat3 m_transformation{};      ///< Transformation matrix for the light source.
        glm::vec4 m_color{};               ///< Color of the light (RGBA).
        glm::vec2 m_innerOuterRadius{};    ///< Inner and outer radius of the light effect.
        float m_intensity{};               ///< Intensity of the light source.
    };

    struct ColliderGridData
    {
        glm::mat3 m_transformation{};      ///< Transformation matrix for the collider grid.
        glm::ivec2 m_gridDimensions{};     ///< Grid dimensions (number of cells in X and Y).
    };

    struct TilemapData {
        glm::mat3 m_transformation{};      ///< Transformation matrix for the tilemap.
        unsigned int m_textureID{};        ///< Texture ID for the tilemap.
        glm::ivec2 m_tilemapDimensions{};  ///< Dimensions of the tilemap (in tiles).
        glm::ivec2 m_tilemapPictureSize{}; ///< Dimensions of the tilemap image (pixels).
        int m_tileIndex{};                 ///< Index of the tile to render.
        int m_layer{};                      ///< Layer for rendering order.
        glm::vec4 m_color{};               ///< Tint color for the tilemap (RGBA).
    };

    struct GraphicsData {
        glm::mat3 m_transformation{};      ///< Transformation matrix for the object.
        unsigned int m_textureID{};        ///< Texture ID for rendering.
        int m_stripCount{};                ///< Number of strips for sprite animations.
        int m_frameNumber{};               ///< Current animation frame number.
        int m_layer{};                     ///< Layer for rendering order.
        glm::vec4 m_color{};               ///< Tint color for the object (RGBA).
    };

    struct DebugDrawData {
        glm::mat3 m_transformation{};      ///< Transformation matrix for the debug shape.
        float m_isCollided{};              ///< Flag indicating collision status (1.0 if collided, 0.0 otherwise).
        int m_shapeType{};                 ///< Shape type (e.g., square, circle, polygon).
    };

    struct TextData {
        std::string m_text;                ///< The text string to render.
        float m_x, m_y;                    ///< Position of the text on the screen.
        float m_rotate{};                  ///< Rotation angle of the text in degrees.
        float m_scale{};                   ///< Uniform scale factor for text size.
        glm::vec2 m_xyScale{};             ///< Non-uniform scale for X and Y.
        int m_layer{};                     ///< Layer for rendering order.
        glm::vec3 m_color{};               ///< Text color (RGB).
        std::string m_fileName;            ///< Font file used for rendering.
        bool m_isCentered;
    };

    struct VideoData
    {
        glm::mat3 transformation; ///< Transformation matrix for video rendering.
        GLuint yTexture;          ///< Texture ID for the Y (luminance) channel.
        GLuint uTexture;          ///< Texture ID for the U (chrominance blue) channel.
        GLuint vTexture;          ///< Texture ID for the V (chrominance red) channel.
        GLint locTransformation;  ///< Shader location for the transformation matrix.
        GLint locView;            ///< Shader location for the view matrix.
        GLint locProjection;      ///< Shader location for the projection matrix.
        GLuint unilayer;          ///< Uniform layer used for multi-layer rendering.
        int layer;                ///< Layer index for rendering order.
    };

    /**
     * @class GraphicsPipe
     * @brief Manages the rendering pipeline, shaders, and drawing functions.
     *
     * The GraphicsPipe class handles the setup and rendering of graphical objects,
     * including mesh data, framebuffers, shaders, and textures. It also provides
     * functionality for instanced rendering, debug drawing, and text rendering.
     */
    class GraphicsPipe
    {
    private:

        /**
         * @struct Mesh
         * @brief Stores mesh data for rendering.
         */
        struct Mesh
        {
            int m_shapeType{};           ///< Shape type (square, circle, etc.).
            unsigned int m_vaoId{};      ///< VAO ID for this mesh.
            unsigned int m_primitiveType{}; ///< OpenGL primitive type (e.g., GL_TRIANGLES).
            unsigned short m_indexElementCount{}; ///< Number of elements to draw.
        };

        /**
         * @brief Sets up the Vertex Array Object (VAO) for rendering text.
         */
        void m_funcSetupTextVao();

        /**
         * @brief Sets up the Vertex Array Object (VAO) for a specific mesh.
         *
         * @param shape Reference to the Mesh structure to set up.
         */
        void m_funcSetupVao(Mesh& shape);

        /**
         * @brief Sets up the Vertex Array Object (VAO) for rendering a grid.
         *
         * Configures the vertex attributes and buffers for rendering grid lines.
         */
        void m_funcSetupGridVao();

        void m_funcSetupVideoVAO();

        /**
         * @brief Generates vertices for a grid and configures spacing.
         *
         * @param vertices A vector to store the generated grid vertices.
         * @param gridSize The number of cells in the grid (per side).
         * @param spacing The spacing between grid lines.
         */
        void m_funcSetupGrid(std::vector<glm::vec3>& vertices, int gridSize, float spacing);

        /**
         * @brief Sets up the Vertex Array Object (VAO) for the framebuffer.
         *
         * This function initializes and configures the VAO needed for rendering
         * the framebuffer. It typically involves binding buffers, defining vertex
         * attributes, and preparing the framebuffer for rendering operations.
         */
        void m_funcSetupFrameBufferVao();

        /**
         * @brief Sets up the Vertex Array Object (VAO) for rendering square outlines.
         */
        void m_funcSetupSquareLinesVao();

        /**
         * @brief Sets up the Vertex Array Object (VAO) for rendering circle outlines.
         *
         * Configures vertex attributes and buffers for drawing circle wireframes.
         */
        void m_funcSetupCircleLinesVao();

        /**
         * @brief Sets up the array buffer for storing model matrix data.
         */
        void m_funcSetupArrayBuffer();

        void m_funcSetupSSBO();


        /**
         * @brief Compiles and links a shader program from vertex and fragment shader sources.
         *
         * @param vertexShader The source code of the vertex shader.
         * @param fragmentShader The source code of the fragment shader.
         * @return The compiled and linked shader program ID.
         */
        unsigned int m_funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader);

        unsigned int m_funcSetupComputerShader(const std::string& computerShader);

        /**
         * @brief Deletes the currently active shader program.
         */
        void m_funcDeleteShader();

        static std::unique_ptr<GraphicsPipe> m_instancePtr; ///< Singleton instance of the GraphicsPipe class.
        // Shader Programs
        unsigned int m_genericShaderProgram{};      ///< Shader program for general rendering.
        unsigned int m_frameBufferShaderProgram{};  ///< Shader program for framebuffer rendering.
        unsigned int m_debugShaderProgram{};        ///< Shader program for debug rendering.
        unsigned int m_textShaderProgram{};         ///< Shader program for text rendering.
        unsigned int m_gridShaderProgram{};         ///< Shader program for rendering grid lines.
        unsigned int m_tilemapShaderProgram{};      ///< Shader program for rendering tilemaps.
        unsigned int m_gridDebugShaderProgram{};    ///< Shader program for rendering collidable grids.
        unsigned int m_lightingShaderProgram{};     ///< Shader program for the lighting pass.
        unsigned int m_finalPassShaderProgram{};    ///< Shader program for the final post-processing pass.
        unsigned int m_particleComputerShaderProgram{};
        unsigned int m_particleShaderProgram{};
        unsigned int m_crtShaderProgram{};

    public:
        unsigned int m_videoShaderProgram{};        ///< Shader program for video
    private:
        // Buffers
        unsigned int m_modelMatrixArrayBuffer{};    ///< Array buffer for model matrices.
        unsigned int m_debugMatrixArrayBuffer{};    ///< Array buffer for debug matrices.
        unsigned int m_vec3Buffer{};                ///< Buffer for storing vec3 vertex attributes.
        unsigned int m_iVec3Buffer{};               ///< Buffer for integer vec3 attributes, used for texture ordering.
        unsigned int m_debugCollisionCheckBuffer{}; ///< Buffer for collision detection in debug drawing.
        unsigned int m_frameBufferObject{};         ///< Framebuffer object for offscreen rendering.
        unsigned int m_multiLightingFrameBufferObject{}; ///< Framebuffer object for lighting pass rendering.
        unsigned int m_multiLightingDepthBufferObject{}; ///< Depth buffer for the lighting framebuffer.
        unsigned int m_additiveLightingFrameBufferObject{}; ///< Framebuffer object for additive lighting pass rendering.
        unsigned int m_additiveLightingDepthBufferObject{}; ///< Depth buffer for the additive lighting framebuffer.
        unsigned int m_crtFrameBufferObject{};
        unsigned int m_crtDepthBufferObject{};
        unsigned int m_finalPassFrameBufferObject{}; ///< Framebuffer object for final post-processing pass.
        unsigned int m_finalPassDepthBufferObject{}; ///< Depth buffer for the final pass framebuffer.
        unsigned int m_depthBufferObject{};         ///< Depth buffer object for storing depth information.
        unsigned int m_gamePreviewFrameBufferObject{};   ///< Framebuffer object for the game preview window.
        unsigned int m_gamePreviewDepthBufferObject{};   ///< Depth buffer for the game preview framebuffer.
        unsigned int m_unlitScreenFrameBufferObject{};
        unsigned int m_unlitScreenDepthBufferObject{};
        unsigned int m_particleSSBO{};
        
        unsigned int m_textBuffer{};                ///< Buffer for text rendering.
        unsigned int m_layerBuffer{};               ///< Buffer for rendering layer data.
        unsigned int m_gridBuffer{};                ///< Buffer for grid vertex data.
        unsigned int m_colorBuffer{};               ///< Buffer for vertex color data.
        unsigned int m_tileIndexBuffer{};           ///< Buffer for tilemap indices.
        unsigned int m_gridColliderBuffer{};        ///< Buffer for grid-based collider data.
        unsigned int m_videoBuffer{};

        glm::mat3 m_testMatrix{};                   ///< Test matrix for rendering.

        /**
         * @brief Compiles an individual shader from source.
         *
         * @param type The type of the shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
         * @param shader The shader source code.
         * @return The compiled shader ID.
         */
        static GLuint m_funcCompileShader(GLuint type, const std::string& shader);

    public:

        int m_unitWidth{ 100 };         ///< The default width for the graphics unit.
        int m_unitHeight{ 100 };        ///< The default height for the graphics unit.
        float m_globalLightIntensity{ 1.f }; ///< The global illumination value.
        const int MAX_PARTICLES = 100000;

        /**
         * @enum ShapeType
         * @brief Enumerates different shape types for rendering.
         */
        enum ShapeType
        {
            SQUARE_LINES, ///< Square outline shape type.
            CIRCLE_LINES  ///< Circle outline shape type.
        };

        /**
         * @brief Destructor for the GraphicsPipe class.
         *
         * Cleans up resources such as framebuffers and shader programs.
         */
        ~GraphicsPipe();

        /**
         * @brief Returns the singleton instance of the GraphicsPipe class.
         *
         * Creates a new instance if it doesn't already exist.
         *
         * @return Pointer to the singleton GraphicsPipe instance.
         */
        static GraphicsPipe* m_funcGetInstance();

        /**
         * @brief Initializes the graphics pipeline, setting up VAOs, shaders, and framebuffers.
         *
         * This function is responsible for enabling blending, setting line width, and initializing
         * various buffers and data structures needed for rendering. It sets up the VAOs for the
         * square, square lines, and text meshes, as well as framebuffers for offscreen rendering.
         * Additionally, it compiles and links shader programs for generic rendering, framebuffer
         * rendering, debug drawing, and text rendering. It reserves memory for storing data for
         * entities, textures, and debugging, ensuring the system can handle the maximum number of entities.
         *
         * Shader programs are loaded for different drawing purposes, and the window size and aspect ratio
         * are obtained from helper functions. Lastly, the function enables the scissor test, used for
         * limiting drawing to a specified area of the window.
         *
         * @note Some code for monitor setup and debug callbacks is commented out but can be uncommented if needed.
         */
        void m_funcInit();

        /**
         * @brief Updates the graphics pipeline.
         *
         * This function should be called every frame to update any dynamic aspects of the rendering pipeline,
         * such as animations, transformations, or other time-dependent changes.
         */
        void m_funcUpdate();

        /**
         * @brief Draws the entire window.
         *
         * Handles rendering all graphical elements to the window, including models, debug information, and text.
         */
        void m_funcDrawWindow();


        /**
        * @brief Sets up the framebuffer for the game preview window.
        *
        * This function initializes and configures a framebuffer object (FBO) specifically
        * for rendering the game preview. It allocates textures for color and depth
        * attachments, binds them to the framebuffer, and ensures the framebuffer is
        * complete for rendering. The framebuffer allows offscreen rendering of the game
        * scene to a texture, which can be displayed in the game preview window.
        *
        * @note This function must be called after the OpenGL context is initialized.
        */
        void m_funcDrawGamePreviewWindow();

        /**
         * @brief Draws all graphical elements.
         *
         * Renders all objects stored in the graphics data structures to the screen.
         */
        void m_funcDraw();

        /**
          * @brief Draws all unlit graphical elements.
          *
          * Renders objects that do not require lighting calculations, such as UI elements.
          */
        void m_funcDrawUnlit();

        /**
         * @brief Draws video textures.
         *
         * Renders video frames stored in textures, applying appropriate transformations.
         */
        void m_funcDrawVideos();


        /**
         * @brief Draws debug elements.
         *
         * Renders debug-specific elements, such as collision boxes or other diagnostic visuals.
         */
        void m_funcDrawDebug();

        /**
         * @brief Renders the game scene to the framebuffer.
         *
         * This function draws the game content into the game framebuffer, which can
         * later be used for post-processing or rendering to a preview window.
         */
        void m_funcDrawGameFrameBuffer();

        /**
         * @brief Draws a grid on the screen.
         *
         * Uses the grid shader and VAO to render a grid for debugging or visualization.
         */
        void m_funcDrawGrid();

        /**
         * @brief Draws a line segment between two points.
         *
         * @param p0 The starting point of the line (in 3D space).
         * @param p1 The ending point of the line (in 3D space).
         */
        void m_funcDrawLine(glm::vec3 p0, glm::vec3 p1);


        /**
         * @brief Renders the grid-based collider visualization.
         *
         * This function draws the grid collider for debugging purposes, allowing
         * visualization of collision boundaries in the scene.
         */
        void m_funcDrawGridCollider();


        /**
         * @brief Draws a tilemap to the screen.
         *
         * Uses the tilemap shader and VAO to render tiles based on the current tilemap data.
         */
        void m_funcDrawTilemap();


        /**
         * @brief Draws the lighting texture to the screen.
         *
         * This function renders the lighting texture generated from the lighting
         * framebuffer onto the screen or another target.
         */
        void m_drawMultiLightingTexture();

        void m_drawAdditiveLightingTexture();

        /**
          * @brief Draws unlit object textures.
          *
          * Renders textures for objects that do not use lighting calculations.
          */
        void m_funcDrawUnlitObjectTexture();

        /**
         * @brief Draws debug textures.
         *
         * Renders debug-related textures, such as wireframes or overlays.
         */
        void m_funcDrawDebugTexture();

        /**
         * @brief Executes the final rendering pass.
         *
         * Combines all rendering layers and outputs the final image to the screen.
         */
        void m_renderFinalPass();


        /**
         * @brief Executes the final rendering pass.
         *
         * Combines the final texture with the CRT shader.
         */
        void m_renderCRTPass();

        /**
         * @brief Executes the final rendering pass with debug overlays.
         *
         * Similar to m_renderFinalPass but includes additional debug information.
         */
        void m_renderFinalPassWithDebug();



        /**
         * @brief Renders the lighting effects in the scene.
         *
         * This function processes and applies lighting effects using the lighting
         * framebuffer, shaders, and associated buffers.
         */
        void m_funcRenderMultiLighting();

        void m_funcRenderAdditiveLighting();

        /**
          * @brief Draws a full-screen quad with a texture.
          *
          * Renders a full-screen quad using the specified texture, often used for post-processing effects.
          *
          * @param texture The texture to be applied to the full-screen quad.
          */
        void m_funcDrawFullScreenQuad(unsigned int texture);

        /**
         * @brief Draws all active particles.
         *
         * Renders the particle system, displaying particles with their respective properties such as position, color, and animation.
         */
        void m_funcDrawParticles();
        /**
         * @brief Sets the drawing mode for rendering.
         *
         * @param mode The OpenGL drawing mode (e.g., GL_TRIANGLES, GL_LINES).
         */
        static void m_funcSetDrawMode(GLenum mode);

        /**
         * @brief Draws text using the text shader.
         *
         * Renders all text elements stored in the text data structures to the screen.
         */
        void m_funcDrawText();
        /**
        * @brief Sets up the framebuffer for offscreen rendering.
        *
        * Allocates and configures a framebuffer object for rendering scenes offscreen.
        */
        void m_funcSetupFrameBuffer();

        /**
         * @brief Sets up the framebuffer for the game preview window.
         *
         * Allocates and configures a framebuffer object for rendering the game preview.
         */
        void m_funcSetupGamePreviewFrameBuffer();


        /**
          * @brief Sets up the final pass buffer.
          *
          * Configures the framebuffer and necessary resources for the final rendering pass.
          */
        void m_funcSetupFinalPassBuffer();

        /**
         * @brief Sets up the framebuffer for unlit screen rendering.
         *
         * Initializes the framebuffer used for rendering unlit objects on the screen.
         */
        void m_funcSetUpUnlitScreenFrameBuffer();
        /**
         * @brief Sets up the framebuffer for lighting rendering.
         *
         * This function configures a framebuffer object (FBO) that will be used for rendering
         * lighting information in a 2D lighting system. It may involve setting up textures
         * and buffers for storing lighting data, such as diffuse, specular, and depth information.
         * The framebuffer will be prepared for subsequent lighting calculations in the rendering pipeline.
         *
         * @note This function assumes that the OpenGL context has been properly initialized.
         */
        void m_funcSetupMultiLightingFrameBuffer();

        void m_funcSetupCRTFrameBuffer();

        void m_funcSetupAdditiveLightingFrameBuffer();

        /**
         * @brief Calculates the model-to-world transformation matrix.
         *
         * Applies object transformations such as translation, rotation, and scaling
         * to generate the model matrix for rendering.
         */
        void m_funcCalculateModelToWorldMatrix();


        /**
        * @brief Renders the game scene to the game preview framebuffer.
        *
        * Prepares the scene by rendering objects, debug visuals, and text into the
        * game preview framebuffer.
        */
        void m_funcRenderGameScene();


        /**
        * @brief Clears internal containers storing graphics data.
        *
        * Empties all vectors holding data for rendering models, text, and debug information.
        */
        void m_funcClearContainers();

        /**
          * @brief Spawns new particles.
          *
          * Generates and initializes new particles based on the emitter's properties.
          */
        void m_spawnParticles();

        /**
         * @brief Updates particle properties.
         *
         * Updates the position, velocity, lifespan, and animation of active particles over time.
         */
        void m_updateParticles();

        //Boolean Values
        bool m_gameMode{ false };

        // Meshes
        Mesh m_squareMesh;              ///< Mesh for square rendering.
        Mesh m_screenMesh;              ///< Mesh for screen rendering.
        Mesh m_squareLinesMesh;         ///< Mesh for rendering square outlines.
        Mesh m_circleLinesMesh;
        Mesh m_textMesh;                ///< Mesh for text rendering.
        Mesh m_lineMesh;
        Mesh m_videoMesh;

        // Matrix Containers
        std::vector<glm::mat3> m_modelMatrix{}; ///< Model transformation matrices.
        std::vector<glm::mat3> m_modelToNDCMatrix{}; ///< Model-to-NDC transformation matrices.
        std::vector<glm::mat3> m_debugBoxToNDCMatrix{}; ///< Debug model-to-NDC matrices for boxes.
        std::vector<glm::mat3> m_debugCircleToNDCMatrix{}; ///< Debug model-to-NDC matrices for circles.

        // Data for rendering
        std::vector<ParticleData> m_particleData{};
        std::vector<EmitterData> m_emitterData{}; ///< Emitter Data for the Scene.
        std::vector<LightingData> m_multiLightingData{}; ///< Lighting data for the scene.
        std::vector<LightingData> m_additiveLightingData{}; ///< Lighting data for the scene.
        std::vector<ColliderGridData> m_colliderGridData{}; ///< Collider grid data for collision checks.
        std::vector<TilemapData> m_tilemapData{}; ///< Data for tilemaps in the scene.
        std::vector<GraphicsData> m_modelData{}; ///< Graphics data for rendering 3D models.
        std::vector<GraphicsData> m_unlitModelData{};
        std::vector<DebugDrawData> m_debugBoxData{}; ///< Data for rendering debug boxes.
        std::vector<TextData> m_textData{}; ///< Data for rendering text elements.
        std::vector<VideoData> m_videoData{};
        std::vector<float> m_debugBoxCollisionChecks{}; ///< Collision check data for debug box rendering.
        std::vector<float> m_debugCircleCollisionChecks{}; ///< Collision check data for debug circle rendering.

        std::vector<glm::vec4> m_colors{}; ///< Colors used in various elements for rendering.
        std::vector<glm::vec4> m_multiLightingColors{}; ///< Lighting colors for illumination effects.
        std::vector<glm::vec3> m_multiLightingParams{}; ///< Parameters related to lighting calculations.
        std::vector<glm::vec4> m_additiveLightingColors{}; ///< Lighting colors for illumination effects.
        std::vector<glm::vec3> m_additiveLightingParams{}; ///< Parameters related to lighting calculations.
        std::vector<std::vector<int>> m_tileIndexes{}; ///< Tile indexes for rendering in the tilemap.
        std::vector<std::vector<int>> m_gridColliderChecks{}; ///< Collider check indexes for the grid.

        std::vector<unsigned int> m_textureIDs{}; ///< Array of texture IDs for rendering.
        std::vector<int> m_layers{}; ///< Layer information for rendering elements.
        std::vector<glm::vec3> m_vec3Array{}; ///< Array of 3D vector data for rendering.
        std::vector<glm::ivec3> m_iVec3Array{}; ///< Array of integer 3D vector data for sprite strip counts and other uses.
        std::vector<int> m_frameNumbers{}; ///< Frame numbers for sprite animations.
        std::vector<image::Image> m_imageData{}; ///< Image data used for rendering textures and sprites.
        std::vector<TilemapData> m_transformedTilemaps{}; ///< Transformed tilemap data after processing.
        std::vector<glm::mat3> m_multiLightingTransforms{}; ///< Multiplicative Lighting transformation matrices for effects.
        std::vector<glm::mat3> m_additiveLightingTransforms{}; ///< Additive Lighting transformation matrices for effects.
        std::vector<std::vector<std::vector<int>>> m_tilemapIndexArrays{}; ///< Tilemap index arrays for 2D grid-based tiles.
        std::vector<std::vector<std::vector<int>>> m_gridColliderArrays{}; ///< Grid collider arrays for collision handling.

        std::vector<glm::ivec3> m_unlitModelParams{};
        std::vector<int> m_unlitLayers{};
        std::vector<glm::vec4> m_unlitColors{};
        std::vector<glm::mat3> m_unlitModelMatrix{};

        unsigned int m_screenTextureVAO{}; ///< Vertex Array Object for screen texture rendering.
        unsigned int m_screenTexture{}; ///< Texture for rendering the screen.
        unsigned int m_gamePreviewTexture{}; ///< Texture for displaying game preview.
        unsigned int m_multiLightingTexture{}; ///< Texture for storing lighting data.
        unsigned int m_additiveLightingTexture{};
        unsigned int m_crtTexture{};
        unsigned int m_finalPassTexture{}; ///< Texture for the final rendering pass.
        unsigned int m_unlitScreenTexture{};

        //Shaders
        const std::string debugVertexShader =
        {
        #include "../Graphics/debugVertexShader.vert"
        };

        const std::string debugFragmentShader =
        {
        #include  "../Graphics/debugFragmentShader.frag"

        };

        const std::string frameBufferVertexShader =
        {
         #include "../Graphics/frameBufferVertexShader.vert"
        };

        const std::string frameBufferFragmentShader =
        {
          #include "../Graphics/frameBufferFragmentShader.frag"
        };


        const std::string genericVertexShader =
        {
         #include "../Graphics/genericVertexShader.vert"
        };

        const std::string genericFragmentShader =
        {
          #include "../Graphics/genericFragmentShader.frag"
        };

        const std::string gridVertexShader =
        {
         #include "../Graphics/gridVertexShader.vert"
        };

        const std::string gridFragmentShader =
        {
          #include "../Graphics/gridFragmentShader.frag"
        };

        const std::string textVertexShader =
        {
         #include "../Graphics/textVertexShader.vert"
        };

        const std::string textFragmentShader =
        {
          #include "../Graphics/textFragmentShader.frag"
        };

        const std::string tilemapVertexShader =
        {
          #include "../Graphics/tilemapVertexShader.vert"
        };

        const std::string tilemapFragmentShader =
        {
          #include "../Graphics/tilemapFragmentShader.frag"
        };

        const std::string gridDebugVertexShader =
        {
          #include "../Graphics/gridDebugVertexShader.vert"
        };

        const std::string gridDebugFragmentShader =
        {
          #include "../Graphics/gridDebugFragmentShader.frag"
        };

        const std::string lightingVertexShader =
        {
          #include "../Graphics/lightingVertexShader.vert"
        };

        const std::string lightingFragmentShader =
        {
          #include "../Graphics/lightingFragmentShader.frag"
        };

        const std::string videoVertexShader =
        {
            #include "../Graphics/videoVertexShader.vert"
        };

        const std::string videoFragmentShader =
        {
            #include "../Graphics/videoFragmentShader.frag"
        };

        const std::string finalPassVertexShader =
        {
            #include "../Graphics/finalPassVertexShader.vert"
        };

        const std::string finalPassFragmentShader =
        {
            #include "../Graphics/finalPassFragmentShader.frag"
        };

        const std::string particleComputerShader =
        {
            #include "../Graphics/particleComputer.comp"
        };

        const std::string particleVertexShader =
        {
            #include "../Graphics/particleVertexShader.vert"
        };

        const std::string particleFragmentShader =
        {
            #include "../Graphics/particleFragmentShader.frag"
        };

        const std::string crtVertexShader =
        {
            #include "../Graphics/crtVertexShader.vert"
        };

        const std::string crtFragmentShader =
        {
            #include "../Graphics/crtFragmentShader.frag"
        };
        
        
};

} // namespace graphicpipe

#endif // GRAPHPIPE_H
