/**
@file
@author  D Burchill <david.burchill@nbcc.ca>
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt
These additions and modifications are my sole work for prog 1266

@section DESCRIPTION


*/
#include "Container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GEX
{

	namespace GUI
	{

		Container::Container() :
			_children(),
			_selectedChild(-1)
		{}

		void Container::pack(Component::Ptr component)
		{
			_children.push_back(component);

			if (!hasSelectableChildren() && component->isSelectable())
				select(_children.size() - 1);
		}

		bool Container::isSelectable() const
		{
			return false;
		}

		void Container::handleEvent(const sf::Event& event)
		{
			// If we have selected a child then give it events
			if (hasSelectableChildren() && _children[_selectedChild]->isActive())
			{
				_children[_selectedChild]->handleEvent(event);
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
				{
					selectPrevious();
				}
				else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
				{
					selectNext();
				}
				else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
				{
					if (hasSelectableChildren())
						_children[_selectedChild]->activate();
				}
			}
		}

		void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			states.transform *= getTransform();

			for (const Component::Ptr& child : _children)
				target.draw(*child, states);
		}

		bool Container::hasSelectableChildren() const
		{
			return _selectedChild >= 0;
		}

		void Container::select(std::size_t index)
		{
			if (_children[index]->isSelectable())
			{
				if (hasSelectableChildren())
					_children[_selectedChild]->deselect();

				_children[index]->select();
				_selectedChild = index;
			}
		}

		void Container::selectNext()
		{
			if (!hasSelectableChildren())
				return;

			// Search next component that is selectable, wrap around if necessary
			int next = _selectedChild;
			do
				next = (next + 1) % _children.size();
			while (!_children[next]->isSelectable());

			// Select that component
			select(next);
		}

		void Container::selectPrevious()
		{
			if (!hasSelectableChildren())
				return;

			// Search previous component that is selectable, wrap around if necessary
			int prev = _selectedChild;
			do
				prev = (prev + _children.size() - 1) % _children.size();
			while (!_children[prev]->isSelectable());

			// Select that component
			select(prev);
		}

	}
}