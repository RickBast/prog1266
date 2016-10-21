/**
@file SceneNode.h
@author  R Bastarache <rick_hockey8@hotmail.com>
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt
These additions and modifications are my sole work for prog 1266

@section DESCRIPTION


*/
#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Command.h"
#include "Category.h"
#include "CommandQueue.h"

namespace GEX
{
	class SceneNode : public sf::Transformable, public sf::Drawable
	{
	public:
		typedef std::unique_ptr<SceneNode> Ptr;

										SceneNode(Category::Type category= Category::None);
										SceneNode(const SceneNode&) = delete;
		SceneNode&						operator=(const SceneNode&) = delete;

		void							onCommand(const Command& command, sf::Time dt);
		virtual unsigned int			getCategory() const;
		void							attachChild(Ptr child);
		Ptr								detachChild(const SceneNode& node);
		void							update(sf::Time dt, CommandQueue& commands);
		sf::Vector2f					getWorldPosition() const;
		sf::Transform					getWorldTransform() const;
		virtual sf::FloatRect			getBoundingRect() const;

	private:

		void							draw(sf::RenderTarget& target, sf::RenderStates state) const override final;
		virtual void					drawCurrent(sf::RenderTarget& target, sf::RenderStates state) const;
		void							drawChildren(sf::RenderTarget& target, sf::RenderStates state) const;
		virtual void					updateCurrent(sf::Time dt, CommandQueue& commands);
		void							updateChildren(sf::Time dt,CommandQueue& commands);
		void							drawBoundingRect(sf::RenderTarget& target, sf::RenderStates state)const;
		SceneNode*						_parent;
		std::vector<Ptr>				_children;
		Category::Type					_defaultCategory;
	};
}

