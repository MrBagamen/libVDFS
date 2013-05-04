#include "../VDFS.hpp"

#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <limits>
#include <iostream>

void playSound(const char* filename, vdfs::Entry& e, sf::SoundBuffer*& sb, sf::Sound& s)
{
	s.stop();
	delete sb;
	std::ifstream file(filename);
	file.seekg(e.offset);
	char* buffer = new char[e.size];
	file.read(buffer, e.size);
	sb = new sf::SoundBuffer;
	sb->loadFromMemory(buffer, e.size);
	delete[] buffer;
	s.setBuffer(*sb);
	s.play();
}

vdfs::word fileIndex(vdfs::File& file, vdfs::word index)
{
	return index - (file.header.entryCount - file.header.fileCount);
}

std::string nowPlaying(vdfs::File& file, vdfs::word index)
{
	auto fi = fileIndex(file, index + 1);
	auto e = file.getEntry(index);
	return "Now playing "  + std::string(e.name, sizeof(e.name)) + " (" + std::to_string(fi) + "/" + std::to_string(file.header.fileCount) + ")";
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Need a filename as argument.\n";
		return 1;
	}
	vdfs::File file;
	
	if (!file.openFromFile(argv[1]))
	{
		return 1;
	}
	
	sf::RenderWindow window(sf::VideoMode(640, 48, 32), "Sound Test");
	sf::Font font;
	font.loadFromFile("sansation.ttf");
	sf::Text text;
	text.setCharacterSize(16);
	text.setFont(font);
	text.setString("< Press Left or Right key >");
	vdfs::word index = file.header.entryCount - file.header.fileCount;
	sf::SoundBuffer* soundBuffer = nullptr;
	sf::Sound sound;
	
	while (window.isOpen())
	{
		sf::Event event;
		
		while (window.pollEvent(event))
		{	
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::Left:
					--index;
					if (fileIndex(file, index) == std::numeric_limits<vdfs::word>::max())
					{
						index = file.header.entryCount - 1;
					}
					{
					auto e = file.getEntry(index);
					text.setString(nowPlaying(file, index));
					playSound(argv[1], e, soundBuffer, sound);
					}
					break;
				case sf::Keyboard::Right:
					++index;
					if (fileIndex(file, index) >= file.header.fileCount)
					{
						index = file.header.entryCount - file.header.fileCount;
					}
					{
					auto e = file.getEntry(index);
					text.setString(nowPlaying(file, index));
					playSound(argv[1], e, soundBuffer, sound);
					}
					break;
				default:
					break;
				}
			}
		}
		
		window.clear(sf::Color(0, 0, 128));
		window.draw(text);
		window.display();
	}
}
