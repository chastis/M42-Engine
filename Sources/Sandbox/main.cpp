
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <Physics/Demo.hpp>

constexpr float cg_speed = 60.f;
constexpr float cg_angular_speed = 15.f;

class rectangle_shape : public sf::Shape
{
	const polygon& shape;
public:
	rectangle_shape(const polygon& rect) : shape(rect)
	{
		update();
	}

	std::size_t getPointCount() const override
	{
		return shape.world_points.size();
	}
	sf::Vector2f getPoint(std::size_t index) const override
	{
		return { shape.world_points[index].x, shape.world_points[index].y };
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Demo");
	window.setFramerateLimit(60);
	//ImGui::SFML::Init(window);

	Demo demo;

	demo.add_rect({320.f, 460.f}, {300.f, 10.f});

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					demo.add_shape({ float(event.mouseButton.x), float(event.mouseButton.y) });
				}
				else
				{
					demo.select_shape(float(event.mouseButton.x), float(event.mouseButton.y));
				}
				break;
			case sf::Event::MouseWheelScrolled:
				g_shapeSize += 2.f * event.mouseWheelScroll.delta;
				break;
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Key::Left)
					demo.set_selected_speed(-cg_speed, demo.get_selected_speed().y);
				else if (event.key.code == sf::Keyboard::Key::Up)
					demo.set_selected_speed(demo.get_selected_speed().x, -cg_speed);
				else if (event.key.code == sf::Keyboard::Key::Right)
					demo.set_selected_speed(cg_speed, demo.get_selected_speed().y);
				else if (event.key.code == sf::Keyboard::Key::Down)
					demo.set_selected_speed(demo.get_selected_speed().x, cg_speed);
				break;
			}
			case sf::Event::KeyReleased:
				demo.set_selected_speed(0.f, 0.f);
			default: break;
			}
		}


		sf::Time dt = deltaClock.restart();

		if (polygon* shape = demo.get_selected())
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
			{
				shape->angle += cg_angular_speed * dt.asSeconds();
			}
			else if (sf::Keyboard::isKeyPressed((sf::Keyboard::E)))
			{
				shape->angle -= cg_angular_speed * dt.asSeconds();
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			demo.add_shape({ float(sf::Mouse::getPosition().x), float(sf::Mouse::getPosition().x) });
		}

		demo.update(dt.asSeconds());

		window.clear();
		demo.visit_shapes([&](const polygon& shape)
			{
				rectangle_shape toDraw(shape);

				toDraw.setFillColor(shape.is_selected ? sf::Color::Red : sf::Color::White);

				window.draw(toDraw);
			});

		window.display();
	}
}
