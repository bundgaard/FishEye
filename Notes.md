
        long ypos = map_up_y(255);

        long xpos = map_up_x(255);

        /*for (long y = ypos; y < ypos + StreamingHeight; y++)
        {
            for (long x = xpos; x < xpos + StreamingWidth; x++)
            {
                auto pixel = pixels[x + y * backgroundSurface->w];

                Uint8 red, green, blue, alpha;
                SDL_GetRGBA(pixel, backgroundSurface->format, &red, &green, &blue, &alpha);
                // std::printf("scale=%d,%d, pixel=%x, color=%d,%d,%d\n", x, y,pixel, red, green, blue);
                // AA RR GG BB

                long index = map_down_x(x) + map_down_y(y) * 64;
                long center_index = 32 + 32 * 64;
                long new_y = center_index + (64.0 * std::sin(20 * 3.14576 * 2 * 50 + y));
                long new_x = center_index + (64.0 * std::cos(20 * 3.14576 * 2 * 50 + x));
                new_y = map_down_y(new_y);
                new_x = map_down_x(new_x);
            //    std::printf("index %d, new %d,%d\n", index, new_x, new_y);
                // framebuf[(x - xpos)+ (new_y - ypos) * 64] = 0xffff1111;//alpha << 24 | red << 16 | green << 8 | blue;
                // framebuf[new_x + new_y * 64] = 0xffff1111;//alpha << 24 | red << 16 | green << 8 | blue;

            }
        }*/



        /*   for (int i = 0; i < 32; i++)
           {
               auto left = pixel(map_up_x(streamingRect.x - i), map_up_y(streamingRect.y));
               framebuf[(32 - i) + 32 * 64] = left;
           }

           for (int i = 0; i < 32; i++)
           {
               auto left = pixel(map_up_x(streamingRect.x + i), map_up_y(streamingRect.y));
               framebuf[(32 + i) + 32 * 64] = left;
           }
*/


        /*  for (int x = 0; x < 32; x++) // move x from 32 to 0
          {
              for (int y = 0; y < 32; y++) // move y from 32 to zero
              {
                  // x=cx+rcosθ,y=cy+rsinθ
                  double phi = atan2(y - 32, x - 32);
                  // std::printf("angle for xy=%d,%d ᵠ=%f\n", x,y, phi * (180.0 / M_PI));
                  // Circle EDGE
                  auto circle_edge_x = 32 + 32 * cos(phi);
                  auto circle_edge_y = 32 + 32 * sin(phi);
                  framebuf[(int) circle_edge_x + (int) circle_edge_y * 64] = 0xff00ff00;
                 // std::printf("circle %f,%f\n", std::ceil(circle_edge_x), std::ceil(circle_edge_y));
                  // NORTHWEST
                  int dx = (int) circle_edge_x - x;
                  int dy = (int) circle_edge_y - y;
                  // std::printf("distance x=%d; distance y=%d\n", dx, dy);
                  int distanceSquared = dx * dx + dy * dy;

                  auto north_west = pixel(map_up_x((streamingRect.x + 32) - x), map_up_y((streamingRect.y + 32) - y));
                  framebuf[(32 - x) + (32 - y) * 64] = north_west;

                  auto north_east = pixel(map_up_x((streamingRect.x + 32) + x), map_up_y((streamingRect.y + 32) - y));
                  framebuf[(32 + x) + (32 - y) * 64] = north_east;

                  auto south_west = pixel(map_up_x((streamingRect.x + 32) - x), map_up_y((streamingRect.y + 32) + y));
                  framebuf[(32 - x) + (32 + y) * 64] = south_west;

                  auto south_east = pixel(map_up_x((streamingRect.x + 32) + x), map_up_y((streamingRect.y + 32) + y));
                  framebuf[(32 + x) + (32 + y) * 64] = south_east;


              }

          }

          for (auto i = 0.0; i < M_PI * 2; i += 0.01)
          {
              int mid_x = 32;
              int mid_y = 32;
              int x = mid_x + (32 * std::cos(i));
              int y = mid_y + (32 * std::sin(i));

              auto foo = atan2(y, x);
              //    std::printf("foo %f\n", foo);

              assert((x + y * 64 < 64 * 64));
              framebuf[x + y * 64] = 0xffff0000;
          }

          framebuf[32 + 32 * 64] = 0xffffffff; // center dot

          SDL_UpdateTexture(streamingTexture, nullptr, framebuf.data(), StreamingWidth * 4);
*/
/*   streamingRect.x += dx;
streamingRect.y += dy;

           if (streamingRect.x < 0 || streamingRect.x + streamingRect.w > 512)
           {
               dx *= -1;
           }
           if (streamingRect.y < 0 || streamingRect.y + streamingRect.h > 512)
           {
               dy *= -1;
           }*/