#define STB_IMAGE_IMPLEMENTATION

#undef main

#include <random>

#include "DefaultGameMode.hpp"
#include "DefaultPlayer.hpp"
#include "Engine.hpp"
#include "StaticMesh.hpp"
#include "StaticMeshComponent.hpp"
#include "ECS/ECSRendererWrapper.hpp"
#include "ECS/ECSSystemWrapper.hpp"
#include "ECS/Entity.hpp"
#include "ECS/System.hpp"
#include "ECS/World.hpp"
#include "ECS/Components/ECSStaticMeshRenderData.hpp"
#include "ECS/Components/ECSTransform.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/EC/CubeObject.hpp"

#define NUM_ENTITIES 10000

void ECSDemo(int argc, char* argv[])
{
    // Engine and scene setup
    Engine* engine = new Engine("ECS Demo", std::filesystem::path(argv[0]).parent_path());
    engine->setGameMode(new DefaultGameMode(engine));

    const auto planeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/plane/plane.obj",
                                                     new Material(new Shader(
                                                         engine->getRuntimePath() / "shaders/blinnPhong/blinnPhong")));
    if (planeImport.empty()) throw std::runtime_error("Plane mesh not found");

    const auto floorObject = new Object(engine);

    StaticMesh* planeMesh = planeImport[0];

    planeMesh->getMaterial()->
               setTextureMap(
                   DIFFUSE,
                   engine->getRuntimePath() / "resources" /
                   "textures/grid.jpeg");
    planeMesh->getMaterial()->setTextureScale({5000.0f, 5000.0f});
    planeMesh->getMaterial()->setAmbient({1.0f, 1.0f, 1.0f});

    const auto floorMeshComponent = new StaticMeshComponent(planeMesh);
    floorMeshComponent->attachTo(floorObject);

    floorObject->setScale({
        5000.0f, 1.0f, 5000.0f
    });
    engine->getScene()->addObject(floorObject);

    const auto skyboxImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/skybox/skybox.obj",
                                                      new Material(
                                                          new Shader(engine->getRuntimePath() / "shaders/sky/sky")));
    if (skyboxImport.empty()) throw std::runtime_error("Skybox mesh not found");

    const auto skyboxObject = new Object(engine);

    StaticMesh* skyboxMesh = skyboxImport[0];
    const auto skyboxMeshComponent = new StaticMeshComponent(skyboxMesh);
    skyboxMeshComponent->attachTo(skyboxObject);
    skyboxObject->setScale({
        50.0f, 50.0f, 50.0f
    });
    engine->getScene()->addObject(skyboxObject);

    const auto player = new DefaultPlayer(engine);
    const auto playerLight = new Light({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 2.5f);
    engine->getScene()->addLight(playerLight);
    playerLight->attachTo(player);
    engine->getScene()->addObject(player);
    engine->possess(player);

    const auto light = new Light({0.0f, 25.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 15.0f);
    engine->getScene()->addLight(light);
    light->attachTo(floorObject);

    engine->getRenderer()->setUnlit(false);
    engine->setCursorVisible(false);

    // ECS Demo
    const auto cubeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/cube/cube.obj",
                                                    new Material(new Shader(
                                                        engine->getRuntimePath() / "shaders/blinnPhong/blinnPhong")));
    if (cubeImport.empty()) throw std::runtime_error("Cube mesh not found");
    StaticMesh* cubeMesh = cubeImport[0];
    if (cubeMesh == nullptr) throw std::runtime_error("Cube mesh is null");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> locationDistribution(-25.0, 25.0);
    std::uniform_real_distribution<> massDistribution(100000.0, 500000.0);

    const glm::vec3 pointGravityLocation = {0.0f, 1.0f, 0.0f};
    constexpr float pointGravityMass = 100000000.0f;
    constexpr float G = 6.67408e-11f;

    const auto world = std::make_shared<World>();

    using CubeEntity = Entity<ECSTransform, RigidBody, ECSStaticMeshRenderData>;

    world->CreateEntities<CubeEntity>(NUM_ENTITIES);


    world->Query<CubeEntity>()->ForEach(
        [cubeMesh, &locationDistribution, &gen, &massDistribution, &pointGravityLocation](auto& entity)
        {
            auto& physicsComponent = entity.template Get<RigidBody>();
            physicsComponent.Mass = massDistribution(gen);

            auto& transform = entity.template Get<ECSTransform>();
            transform.Location += glm::vec3(locationDistribution(gen), 1.0f, locationDistribution(gen));
            transform.Scale = glm::vec3(glm::mix(0.01f, 0.5f,
                                                 glm::clamp((physicsComponent.Mass - 100000.0f) / 500000.0f, 0.0f,
                                                            1.0f)));

            const glm::vec3 radius = pointGravityLocation - transform.Location;
            const float distance = length(radius);
            physicsComponent.Velocity = sqrt(G * pointGravityMass / distance) * normalize(radius);

            entity.template Get<ECSStaticMeshRenderData>().Mesh = cubeMesh;
        });

    // Physics system that moves the cubes
    auto physicsSystem = std::make_unique<System<CubeEntity>>(
        System<CubeEntity>(world));

    physicsSystem->RegisterFunction([&](float deltaTime, auto& entity)
    {
        RigidBody& physicsComponent = entity.template Get<RigidBody>();
        auto& transformComponent = entity.template Get<ECSTransform>();

        const glm::vec3 radius = pointGravityLocation - transformComponent.Location;
        const float distance = length(radius);

        const float forceValue = glm::clamp(G * pointGravityMass * physicsComponent.Mass /
                                            glm::pow(distance, 2.0f), 0.0f, 1.0f);
        physicsComponent.Force = forceValue * normalize(
            radius);

        physicsComponent.Acceleration = physicsComponent.Force / physicsComponent.Mass;
        physicsComponent.Velocity += physicsComponent.Acceleration * deltaTime;

        transformComponent.Location += physicsComponent.Velocity * deltaTime;
    });

    auto* physicsSystemWrapper = new ECSSystemWrapper(engine, std::move(physicsSystem));
    engine->getScene()->addObject(physicsSystemWrapper);

    // Rendering system that renders the entities
    auto renderingSystem = std::make_unique<System<CubeEntity>>(
        System<CubeEntity>(world));

    renderingSystem->RegisterFunction([](float deltaTime, auto& entity)
    {
        auto& transformComponent = entity.template Get<ECSTransform>();
        auto& renderDataComponent = entity.template Get<ECSStaticMeshRenderData>();

        if (renderDataComponent.Mesh == nullptr)
        {
            return;
        }

        Transform transform;
        transform.setLocation(transformComponent.Location);
        transform.setRotation(transformComponent.Rotation);
        transform.setScale(transformComponent.Scale);

        renderDataComponent.Mesh->draw(transform);
    });

    auto* renderingSystemWrapper = new ECSRendererWrapper(engine, std::move(renderingSystem));
    engine->getScene()->addObject(renderingSystemWrapper);

    engine->run();

    delete engine;
}

void ECDemo(int argc, char* argv[])
{
    // Engine and scene setup
    Engine* engine = new Engine("ECS Demo", std::filesystem::path(argv[0]).parent_path());
    engine->setGameMode(new DefaultGameMode(engine));

    const auto planeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/plane/plane.obj",
                                                     new Material(new Shader(
                                                         engine->getRuntimePath() / "shaders/blinnPhong/blinnPhong")));
    if (planeImport.empty()) throw std::runtime_error("Plane mesh not found");

    const auto floorObject = new Object(engine);

    StaticMesh* planeMesh = planeImport[0];

    planeMesh->getMaterial()->
               setTextureMap(
                   DIFFUSE,
                   engine->getRuntimePath() / "resources" /
                   "textures/grid.jpeg");
    planeMesh->getMaterial()->setTextureScale({5000.0f, 5000.0f});
    planeMesh->getMaterial()->setAmbient({1.0f, 1.0f, 1.0f});

    const auto floorMeshComponent = new StaticMeshComponent(planeMesh);
    floorMeshComponent->attachTo(floorObject);

    floorObject->setScale({
        5000.0f, 1.0f, 5000.0f
    });
    engine->getScene()->addObject(floorObject);

    const auto skyboxImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/skybox/skybox.obj",
                                                      new Material(
                                                          new Shader(engine->getRuntimePath() / "shaders/sky/sky")));
    if (skyboxImport.empty()) throw std::runtime_error("Skybox mesh not found");

    const auto skyboxObject = new Object(engine);

    StaticMesh* skyboxMesh = skyboxImport[0];
    const auto skyboxMeshComponent = new StaticMeshComponent(skyboxMesh);
    skyboxMeshComponent->attachTo(skyboxObject);
    skyboxObject->setScale({
        50.0f, 50.0f, 50.0f
    });
    engine->getScene()->addObject(skyboxObject);

    const auto player = new DefaultPlayer(engine);
    const auto playerLight = new Light({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 2.5f);
    engine->getScene()->addLight(playerLight);
    playerLight->attachTo(player);
    engine->getScene()->addObject(player);
    engine->possess(player);

    const auto light = new Light({0.0f, 25.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 15.0f);
    engine->getScene()->addLight(light);
    light->attachTo(floorObject);

    engine->getRenderer()->setUnlit(false);
    engine->setCursorVisible(false);

    // classic EC Demo
    const auto cubeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/cube/cube.obj",
                                                    new Material(new Shader(
                                                        engine->getRuntimePath() / "shaders/blinnPhong/blinnPhong")));
    if (cubeImport.empty()) throw std::runtime_error("Cube mesh not found");
    StaticMesh* cubeMesh = cubeImport[0];
    if (cubeMesh == nullptr) throw std::runtime_error("Cube mesh is null");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> locationDistribution(-25.0, 25.0);
    std::uniform_real_distribution<> massDistribution(100000.0, 500000.0);

    const glm::vec3 pointGravityLocation = {0.0f, 1.0f, 0.0f};
    constexpr float pointGravityMass = 100000000.0f;

    for (auto i = 0; i < NUM_ENTITIES; ++i)
    {
        const auto cubeObject = new CubeObject(engine, cubeMesh, massDistribution(gen), pointGravityLocation,
                                               pointGravityMass);
        cubeObject->setLocation({locationDistribution(gen), 1.0f, locationDistribution(gen)});

        engine->getScene()->addObject(cubeObject);
    }

    engine->run();

    delete engine;
}


int main(int argc, char* argv[])
{
    ECSDemo(argc, argv);
    //ECDemo(argc, argv);

    return 0;
}
