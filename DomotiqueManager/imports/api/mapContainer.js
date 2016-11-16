import { Mongo } from 'meteor/mongo';
 
export const MapContainer = new Mongo.Collection('mapContainer');

if (Meteor.isServer) {
   Meteor.publish('mapContainer', function mapContainerPublication() {
      console.log('PUBLISHING the map')
     return MapContainer.find({}, {sort: {createdAt: -1}, limit:2 })
  });
}