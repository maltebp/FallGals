#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

struct SDL_KeyboardEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseMotionEvent;

namespace fg::core {

    class GameTime;
    class Renderer;
    

    class Entity {
    public:

        Entity(const Entity&) = delete;

        ~Entity();
    
        void update(const GameTime& time);

        void render(Renderer& renderer);

        void renderGui();

        // Returns true if event is handled
        bool keyEvent(SDL_KeyboardEvent& keyboardEvent);

        // Returns true if event is handled
        bool mouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent);

        // Returns true if event is handled
        bool mouseButtonEvent(SDL_MouseButtonEvent& mouseButtonEvent);

        // Mark this entity for destruction. It will not actually be destroyed
        // before its parent is cleaned, and even then it is only removed from
        // this Entity's hierarchy and reference.
        void destroy();

        // Call destroy on all children of this entity
        void destroyAllChildren();

        // Deletes this and all child entities marked for destruction
        void clean();

        const std::vector<std::shared_ptr<Entity>>& getChildren() const;

        // The copy is to avoid outside access to the internal children vector.
        // A better (but more complex) approach would be to expose children via
        // enumeration.
        std::vector<std::shared_ptr<Entity>> getChildrenCopy();

        // Calls given callback for all children (recursively) of a given type,
        // using a depth-first search method.
        template<class TEntity>
        void forAllChildrenOfType(std::function<void(const std::shared_ptr<TEntity>&)> callback) const {
            static_assert(
                std::is_convertible<TEntity*, Entity*>::value,
                "TEntity must publicly inherit from Entity"
            );

            for( const std::shared_ptr<Entity>& child : children ) {
                child->forAllChildrenOfType<TEntity>(callback);
                std::shared_ptr<TEntity> castedChild = std::dynamic_pointer_cast<TEntity>(child);
                if (castedChild == nullptr) continue;
                callback(castedChild);
            }
        }

    protected:

        Entity();

        template<class TEntity, typename ... TArgs>
        std::shared_ptr<TEntity> createChild(TArgs ... constructionArgs) {
            static_assert(
                std::is_convertible<TEntity*, Entity*>::value,
                "TEntity must publicly inherit from Entity"
            );
            
            std::shared_ptr<TEntity> entity = std::make_shared<TEntity>(constructionArgs...);
            children.push_back(entity);
            return entity;
        }

        virtual void onUpdate(const GameTime& time) { }

        virtual void onRender(Renderer& renderer) { }

        virtual void onRenderGui() { }

        // Returns true if event is handled
        virtual bool onKeyEvent(SDL_KeyboardEvent& keyboardEvent) { return false; }

        // Returns true if event is handled
        virtual bool onMouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent) { return false; }

        // Returns true if event is handled
        virtual bool onMouseButtonEvent(SDL_MouseButtonEvent& mouseButtonEvent) { return false; }

    private:             

        std::vector<std::shared_ptr<Entity>> children;

        bool isMarkedForDestruction = false;

    };

}