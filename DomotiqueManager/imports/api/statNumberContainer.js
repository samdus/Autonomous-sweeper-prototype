import { Mongo } from 'meteor/mongo';
 
export const StatContainer = new Mongo.Collection('statNumberContainer');

if (Meteor.isServer) {
  Meteor.publish('statNumberContainer', function statContainerPublication(collectionName) {
    console.log('PUBLISHING ' +collectionName)
     return StatContainer.find({statIdentifier: collectionName})
  });
}