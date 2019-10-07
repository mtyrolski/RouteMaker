/** @file
 * Class interface storing the map of national roads
 *
 * @author Łukasz Kamiński <kamis@mimuw.edu.pl>, Marcin Peczarski
 * <marpe@mimuw.edu.pl>, Michał Tyrolski <michal.tyrolski@gmail.com>
 * @copyright Uniwersytet Warszawski
 * @date 20.03.2019
 */

#ifndef __MAP_H__
#define __MAP_H__

#include <stdbool.h>
#include "city.h"
#include "int_list.h"
#include "queue.h"
#include "route.h"
#include "utils.h"
#include "config.h"

/**
 * Struct which contains data about country routes
 */
typedef struct Map
{
    unsigned int citySize;  ///< size of map
    City** cities;          ///< array of cities
    Tree* ramp;             ///< tree for access to cities in log time
    LinkedRoad* roads;      ///< linked list of roads
    LinkedRoute* routes;    ///< linked list of routes
} Map;

/** @brief creates a new structure
 * Creates a new, empty structure that does not contain any cities, or road
 * sections
 * @return Pointer to structure or NULL if cannot allocate memory
 */
Map* newMap(void);

/** @brief Deletes structure.
 * Deletes structure indicated by @p map.
 * It does nothing if the pointer is NULL.
 * @param[in] map        – pointer for deleting structure.
 */
void deleteMap(Map* map);

/** @brief Adds a stretch of road to the map between two different cities.
 * If one of the given cities does not exist, it adds it to the map, then
 * adds a section of the road between these cities to the map.
 * @param[in, out] map - an indicator on the structure storing the road map;
 * @param[in] city1 - indicator for the inscription representing the name of the
 * city;
 * @param[in] city2 - indicator for the inscription representing the name of the
 * city;
 * @param[in] length - length in km of the road section;
 * @param[in] builtYear - the year of construction of the road section.
 * @return The value of @p true if a stretch of road has been added.
 * The value of @p false, if an error occurred: one of the parameters is
 * incorrect value, both given city names are identical, the distance between
 * them cities already exist or did not manage to allocate memory.
 */
bool addRoad(Map* map, const char* city1, const char* city2, unsigned length,
             int builtYear);

/** @brief Modifies the year of the last repair of the road section.
 * For a section of a road between two cities, it changes the year of its last
 * renovation or sets this year if the episode has not been repaired yet.
 * @param[in, out] map - an indicator on the structure storing the road map;
 * @param[in] city1 - indicator for the inscription representing the name of the
 * city;
 * @param[in] city2 - indicator for the inscription representing the name of the
 * city;
 * @param[in] repairYear - the year of the last renovation of the road section.
 * @return The value of @p true if the modification was successful.
 * The value of @p false, if an error occurred: one of the parameters is
 * incorrect value, one of the given cities does not exist, there is no stretch
 * of road between given cities, the given year is earlier than recorded for
 * this episode expensive construction year or last renovation.
 */
bool repairRoad(Map* map, const char* city1, const char* city2, int repairYear);

/** @brief Connects two different cities by national road.
 * Creates a national road between two cities and gives it the given number.
 * Among the existing road sections, it searches for the shortest route. if
 * more than one way of such a choice, it determines for each variant
 * among the road sections selected in it, the one that was most recently built
 * or renovated and chooses the variant with the segment, which is the youngest.
 * @param[in, out] map - an indicator on the structure storing the road map;
 * @param[in] routeId - national road number;
 * @param[in] city1 - indicator for the inscription representing the name of the
 * city;
 * @param[in] city2 - pointer to the inscription representing the name of the
 * city.
 * @return Value @p true if a national road has been created.
 * The value of @p false, if an error occurred: one of the parameters is
 * incorrect value, there is already a national road with the given number, one
 * of the given cities do not exist, both given city names are identical, you
 * can not unambiguously determine the national road between the given cities or
 * failed to allocate memory.
 */
bool newRoute(Map* map, unsigned routeId, const char* city1, const char* city2);

/** @brief Extends the national road to the given city.
 * Adds new sections of roads to the given city to the national road in a way
 * that to make the new fragment of the national road the shortest. If there is
 * more than one the method of such extension, it determines for each variant
 * among added ones road sections, the one that was most recently built or
 * renovated and chooses variant with an episode that is the youngest.
 * @param[in, out] map - an indicator on the structure storing the road map;
 * @param[in] routeId - national road number;
 * @param[in] city - a pointer to the inscription representing the name of the
 * city.
 * @return Value @p true if the national road has been extended.
 * The value of @p false, if an error occurred: one of the parameters is
 * incorrect name, there is no national road with the given number, there is no
 * city with the given number name, the national road with the given number is
 * already passing through the given city, the given national road ends in the
 * given city, it can not be clearly defined designate a new section of the
 * national road or failed to allocate memory.
 */
bool extendRoute(Map* map, unsigned routeId, const char* city);

/** @brief Removes a stretch of road between two different cities.
 * Removes a stretch of road between two cities. If removing this stretch of
 * road causes interruption of some national road, it complements it existing
 * road sections in such a way as to be the shortest. if more than one way of
 * supplementing it, for each variant marks among the added road sections the
 * one that was most recently built or renovated and chooses the variant with
 * the segment that is the youngest.
 * @param[in, out] map - an indicator on the structure storing the road map;
 * @param[in] city1 - indicator for the inscription representing the name of the
 * city;
 * @param[in] city2 - pointer to the inscription representing the name of the
 * city.
 * @return Value @p true if the road section has been removed.
 * The value of @p false, if this road segment can not be removed due to an
 * error: one of the parameters has an incorrect value, there is no one of the
 * given cities, there is no road between the given cities, it can not be
 * clearly defined complete the interrupted domestic road or failed to allocate
 * memory.
 */
bool removeRoad(Map* map, const char* city1, const char* city2);

/** @brief Provides information about the national road.
 * Returns the pointer to an inscription that contains information about the
 * national road. allocates memory for this inscription. Returns an empty string
 * if there is no national road with the given number. The allocated memory must
 * be released using the free function. Information is printed in the following
 * format: national road number, city name, length of the road section,
 * construction year or last renovation, name of the city, length of the road
 * section, construction year or last renovation, city name; ... city name. The
 * order of the cities on the list is such that the cities @p city1 and @p
 * city2, given in calling the @ref newRoute function that created this national
 * road have been listed in this order.
 * @param[in, out] map - an indicator on the structure storing the road map;
 * @param[in] routeId - national road number.
 * @return Indicator for writing or NULL when memory could not be allocated.
 */
char const* getRouteDescription(Map* map, unsigned routeId);

/**
 * @brief Updates the map by adding a new national road to it. The national road
 * is built on the basis of lists of cities, the length of roads between
 * them and their construction years. All operations are performed based
 * on built-in functions such as addRoad. If some function used in
 *constructNewRoute return false then we are not going to do any change (unless
 *it means memory fail). We do not withdraw changes made via function calls from
 *the map module (we are going to do them - it means that input was correct and
 *any problem (not including memory) will not take place. If the call was
 *successful, the changes in the map remain. In particular, this means that an
 *attempt to add a national road that includes a cycle will do nothing. It also
 * means that trying to add road which has different length with existing one,
 *or trying road which has older year of build will cause an error. If input
 *format is correct, Any problem is not expected, then we are going to start
 * adding elements to route. If we have an problem with memory, then we add all
 *changes to map (all cities and roads which we added before caught and memory
 *problem) but then we are not going to create formal route. If function failed
 *does not change the observable map state and results in false.
 * @param[in, out] map - an indicator on the structure storing the road map;
 * @param[in] cities - pointer to tab of cities
 * @param[in] lengths - pointer to tab of lengths
 * @param[in] years - pointer to tab of years
 * @param[in] roadsSize - number of roads
 * @param[in] routeNumber - potential number of route
 * @param[in] finishRoad - name of city which cause a cycle (can be NULL) - then
 *route is being built to finish road but without it
 * @see addRoad
 * @return true if route was fully constructed - false if route was
 *part-constructed (but not added to map) memory could not be allocated,
 *arguments were not correct
 */
bool constructNewRoute(Map* map, char** cities, unsigned int* lengths,
                       int* years, int roadsSize, unsigned int routeNumber,
                       char* finishRoad);

/**
 * @brief Removes from the road map the road with the given number, if it
 * exists, giving the result true, and otherwise, i.e. if the specified national
 * road does not exist or the given number is incorrect, nothing changes in the
 * road map, resulting in false. It does not remove road roads or cities.
 * @param[in, out] map - an indicator on the structure storing the road map;
 * @param[in] routeId - potential number of route
 * @return true if route was removed, false otherwise
 */
bool removeRoute(Map* map, unsigned routeId);

#endif /* __MAP_H__ */